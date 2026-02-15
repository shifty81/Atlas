# Atlas Engine — GUI System Architecture

Atlas uses a single, unified GUI system for the editor, game UI, and headless server.
There is no ImGui dependency — every widget, layout solver, and event path is custom-built
and designed for deterministic replay from day one.

## Design Philosophy

- **State-first, not render-first.** The GUI tree is a data structure that exists
  independently of any renderer. Headless mode runs the full UI tree with no
  draw calls; the renderer is an optional consumer of that tree.
- **One system, three contexts.** Editor panels, in-game HUD, and headless server
  UI share the same widget set, layout solver, and event bus. The difference
  between them is authority, permissions, and mounting — not technology.
- **Deterministic and replayable.** Every GUI interaction produces a semantic event
  that can be recorded, replayed, and diffed. CI pipelines can run UI replays
  and export video without human interaction.
- **Declarative and version-controlled.** Layouts are described in a text-based DSL
  that is diff-friendly and supports schema migration between versions.

## Widget System

Widgets live in `engine/ui/` and are organized around two complementary
abstractions: a flat widget registry (`UIScreenGraph`) and a node-graph
evaluation model (`UIGraph`).

### Widget Types

| Type | Category | Description |
|------|----------|-------------|
| Panel | Container | Rectangular region; root of a widget subtree |
| Button | Interactive | Click target with pressed/released semantics |
| Text | Display | Static or data-bound text label |
| Image | Display | Texture-backed rectangular region |
| List | Container | Ordered vertical collection of children |
| SlotGrid | Container | Grid of discrete item slots (inventories, toolbars) |
| InputField | Interactive | Editable text with cursor and selection |

These types are declared in the `UIWidgetType` enum (`UIScreenGraph.h`).

### Rendering Primitives

All widgets decompose into a small set of render primitives: **rect**, **text**,
**icon**, **border**, and **image**. The renderer is only responsible for
drawing these primitives — it never interprets widget semantics.

### Node-Graph Evaluation

`UIGraph` (`UIGraph.h`) exposes a DAG of `UINode` objects connected by typed
ports (`Float`, `Bool`, `String`, `Layout`, `Action`). The graph is compiled
into a topological execution order and evaluated once per frame via
`UIGraph::Execute`. Cycle detection and edge-type validation run at compile
time.

```
UIGraph::Compile()          — topological sort, cycle + type check
UIGraph::Execute(UIContext) — evaluate nodes in sorted order
```

Built-in node types (`UINodes.h`):

| Node | Category | Purpose |
|------|----------|---------|
| PanelNode | Container | Sized rectangular region |
| ButtonNode | Interactive | Click detection, action output |
| TextNode | Display | Text content with data binding |
| ListNode | Container | Dynamic child collection |

## Screen Graph

`UIScreen` (`UIScreenGraph.h`) is a flat, ID-keyed widget registry that tracks
parent–child relationships, visibility, and spatial layout. It is the runtime
counterpart of the node graph — once a `UIGraph` is compiled and evaluated, the
results are materialized into a `UIScreen` for hit-testing and rendering.

```cpp
uint32_t id = screen.AddWidget(UIWidgetType::Button, "fire", x, y, w, h);
screen.SetParent(id, toolbarId);
screen.SetVisible(id, true);
```

## Layout Solver

### Integer-Only Math

All layout arithmetic uses integer coordinates and ratio-based splits
expressed as rational numbers. Floating-point is used only at the final
rasterization step. This guarantees bit-identical layout across platforms and
replay sessions.

### Formal Invariants

The layout solver enforces these invariants at every frame:

1. **No overlap** — sibling widgets never share pixels.
2. **Full coverage** — the union of siblings exactly fills the parent.
3. **Min-size respect** — no widget is smaller than its declared minimum.
4. **Deterministic docking** — the same sequence of dock mutations always
   produces the same layout.

### Constraint System

Panel sizing is expressed as constraints (minimum, preferred, maximum) with
ratio-based weights for flexible space distribution. The solver resolves
constraints in a single pass over the dock tree.

### Dock Mutation Operations

| Operation | Effect |
|-----------|--------|
| Split | Divide a node into two children (horizontal or vertical) |
| Merge | Collapse two siblings back into their parent |
| Resize | Adjust the split ratio between siblings |
| Move | Detach a panel and re-dock it at a new location |

## Editor Framework

Editor-specific layout lives in `editor/ui/` and builds on top of the engine
widget system.

### EditorPanel

`EditorPanel` (`EditorPanel.h`) is the base class for every editor panel. It
exposes a name, a visibility flag, and a `Draw()` entry point.

```cpp
class EditorPanel {
public:
    virtual const char* Name() const = 0;
    virtual void Draw() = 0;
    virtual bool IsVisible() const;
    virtual void SetVisible(bool visible);
};
```

### DockNode

`DockNode` (`DockNode.h`) is a binary tree node that describes a recursive
split layout. Each node is either a leaf holding an `EditorPanel*` or an
internal node with a split direction and ratio.

```cpp
struct DockNode {
    DockSplit split;      // None | Horizontal | Vertical
    float splitRatio;     // 0.0–1.0
    std::unique_ptr<DockNode> a;
    std::unique_ptr<DockNode> b;
    EditorPanel* panel;   // non-null only for leaves
};
```

### EditorLayout

`EditorLayout` (`EditorLayout.h`) owns the root `DockNode` and a flat list of
registered panels. It drives the recursive `DrawNode` traversal each frame.

## GUI DSL

Layouts are described in a declarative, text-based DSL that is designed for
version control and code review.

### Syntax Overview

```
layout "DefaultEditor" {
    split horizontal 0.25 {
        panel "ToolShelf"
        split vertical 0.70 {
            panel "SceneView"
            panel "Properties"
        }
    }
    dock bottom 0.30 {
        tabs {
            panel "Console"
            panel "Timeline"
            panel "NodeGraph"
        }
    }
}
```

### Capabilities

- **Docking targets** — panels declare where they can be docked (top, bottom,
  left, right, center, float).
- **Tab groups** — multiple panels share a single dock slot with tab switching.
- **Splits** — horizontal and vertical splits with explicit ratios.
- **Migration** — a version header enables automatic migration when the DSL
  schema changes between engine releases.

## Event System

### Semantic Input Recording

The event system records **semantic** UI events — "button `inventory_open`
pressed" — not raw pixel coordinates. Events reference widgets by stable UI ID,
so replays remain valid even if the layout changes.

### UICommandBus

All UI events flow through a thread-safe, lock-free `UICommandBus`. Producers
(input thread, network, replay) enqueue commands; the UI thread drains and
dispatches them once per frame. This guarantees ordering and avoids
data races without heavy synchronization.

### Event Flow

```
Input Source → UICommandBus → UI Thread Drain → Widget Dispatch → State Update
```

## GUI Replay

### Recording

The GUI input recorder captures every semantic event with its tick number and
target widget ID. The recording is a flat, append-only log that can be stored
alongside simulation replays.

### Playback

Playback feeds the recorded events back through the `UICommandBus` at the
correct tick. Because the layout solver and widget evaluation are deterministic,
playback reproduces the exact UI state frame-by-frame.

### CI Integration

- Automated tests can run UI replays in headless mode.
- A post-replay pass can export per-frame screenshots or video for visual
  regression testing.
- Deterministic font rasterization is enforced (fixed glyph atlas, integer
  metrics) so pixel-level comparison is stable across machines.

## ABI Compatibility

### GUI ABI Lock

The GUI exposes a locked ABI surface so that pre-compiled panels and plugins
can be loaded without recompilation when the engine is updated. The ABI covers:

- Widget type enum values
- Event struct layouts
- UICommandBus enqueue/dequeue signatures

### Versioning

- GUI asset schemas carry a version number and are validated on load.
- Breaking changes require a version bump and a migration path in the DSL
  loader.

## File Locations

| Path | Contents |
|------|----------|
| `engine/ui/UIGraph.h/.cpp` | Node-graph evaluation model (DAG of UINodes) |
| `engine/ui/UINodes.h/.cpp` | Built-in widget nodes (Panel, Button, Text, List) |
| `engine/ui/UIScreenGraph.h/.cpp` | Flat widget registry with parent–child tracking |
| `editor/ui/EditorPanel.h` | Base class for editor panels |
| `editor/ui/EditorLayout.h/.cpp` | Dock-tree layout manager |
| `editor/ui/DockNode.h` | Binary split tree node for docking |
