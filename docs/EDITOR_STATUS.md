# Atlas Editor — Status & Gap Analysis

> This document provides an honest assessment of the Atlas Editor's completion
> status, what works today, and what remains to be done before the editor is
> fully functional.

---

## Overview

The Atlas Editor is one of four executables built from the engine core. It is
designed to be a Blender-style authoring environment with dockable panels,
graph editing, and AI-assisted workflows.

**Current state:** The editor's **logic layer is solid** — 14+ panels have
real, functional implementations. The **rendering layer is deferred** — all
`Draw()` methods are no-ops awaiting a UI backend (ImGui, custom, or
terminal).

---

## What Works Today ✅

### Panels with Functional Logic

| Panel | Key Features | Status |
|-------|-------------|--------|
| **Console** | Command parsing, entity spawning, ECS dump, net queries | ✅ Full |
| **World Graph Editor** | Node palette, canvas, inspector, compile/preview | ✅ Full |
| **ECS Inspector** | Entity snapshots, component listing, state blocks | ✅ Full |
| **Asset Browser** | Registry scanning, filtering, sorting, selection | ✅ Full |
| **Profiler** | Frame timing history, system metrics, average/peak | ✅ Full |
| **Network Inspector** | Mode, peer count, RTT, connection snapshots | ✅ Full |
| **Project Picker** | Recent projects list, project loading | ✅ Full |
| **Voice Commands** | Listen state, phrase processing, intent routing | ✅ Full |
| **Replay Timeline** | Markers, divergence detection, frame branching | ✅ Full |
| **CI Dashboard** | Pipeline runs, check results, pass/fail tracking | ✅ Full |
| **Job Trace** | Execution order, determinism checking | ✅ Full |
| **State Hash Diff** | Hash comparison, component breakdown, divergence detail | ✅ Full |
| **Save File Browser** | Directory scanning, file metadata, selection | ✅ Full |
| **Proof Viewer** | TLA+ syntax highlighting, tokenizer | ✅ Full |

### Infrastructure

| Component | Status |
|-----------|--------|
| `EditorPanel` base class | ✅ Visibility, closable flags, name |
| `EditorLayout` | ✅ Panel registry, tree traversal rendering |
| `DockNode` tree | ✅ Recursive splits, split ratio, panel assignment |
| `EditorAttachProtocol` | ✅ Standalone, client-attach, server-attach modes |
| `CommandHistory` | ✅ Undo/redo with execute/undo stacks |

---

## What Needs Work 🔧

### Rendering Layer (Critical Path)

All `Draw()` methods follow this pattern:

```cpp
void SomePanel::Draw() {
    // Display state is maintained by logic methods.
    // A full UI backend would render the data here.
}
```

**What this means:** The panels compute and maintain their state correctly, but
nothing appears on screen. The engine needs a concrete UI rendering backend.

**Options for rendering backend:**
1. **Dear ImGui** — Most common for C++ game engine editors. Low integration cost.
2. **Custom Atlas renderer** — Uses the existing `UIGraph` and `UILayoutSolver`.
   More work, but self-hosting the editor in Atlas's own GUI system is a project goal.
3. **Terminal UI (TUI)** — For headless/server environments. The `HeadlessGUI`
   class already exists as a foundation.

### Editor Main Loop

The `editor/main.cpp` entry point:
- ✅ Initializes engine correctly (Core, Render, UI, ECS, Networking, Editor)
- ✅ Uses `EditorAttachProtocol` for mode selection
- 🔧 `BuildEditorUI()` creates a hard-coded 1280×720 pixel layout
- 📋 Should dynamically build UI from DSL or saved layout

### AI Assistant

| Component | Status | Gap |
|-----------|--------|-----|
| `AtlasAICore` | ✅ Intent registry, permissions, request routing | No LLM backend |
| `AtlasAssistantPanel` | ✅ Prompt/suggestion UI framework | Suggestions are placeholder |
| `AIDiffViewerPanel` | ✅ Hunk accept/reject workflow | No generated diffs |
| `EditorAssistant` | ✅ Router for explain/suggest/mutate | Returns hardcoded text |
| `AssetGraphAssistant` | ✅ Method signatures | Returns empty results |
| `AIBackend` | 📋 Virtual base class defined | No implementations |

### Game Packager

- ✅ UI scaffolding (target selection, build mode, settings)
- 📋 **No actual build/cook logic** — pressing "Build" does nothing
- Needs integration with `AssetCooker`, `BuildProfile`, and `PlatformTarget`

---

## What's Fully Missing ❌

| Feature | Description | Blocking? |
|---------|-------------|-----------|
| **Play-In-Editor** | ✅ Implemented — Simulate, Pause, Step, Possess Entity, Client-Server Loopback, State Restore | Done |
| **Material Graph Editor** | Visual material/shader authoring | No — can use text for now |
| **Quest/Story Editor** | Visual narrative authoring UI | No |
| **Inventory Editor** | Item management UI | No |
| **Permission Enforcement** | Attach protocol tiers not enforced | Medium |
| **Layout Persistence** | ✅ Implemented — Save/restore panel arrangement to/from JSON | Done |
| **Live Edit Rules** | Config hot-reload gating, schema rebuild | Low |

---

## Path to a Fully Functional Editor

### Phase A — Minimum Viable Editor (Weeks)

1. Integrate a UI rendering backend (Dear ImGui recommended)
2. Wire `Draw()` methods to render their maintained state
3. Replace hard-coded layout with DSL-driven or saved layout
4. Verify all 14+ panels render correctly

### Phase B — Core Workflows (Weeks)

1. Implement Play-In-Editor (simulate mode)
2. Wire Game Packager to AssetCooker + BuildProfile
3. Connect AI assistant to a local LLM or API endpoint
4. Add layout save/restore

### Phase C — Polish (Months)

1. Implement remaining editor panels (material, quest, inventory)
2. Permission tier enforcement
3. Unreal-grade aesthetics (dark theme, icons, typography)
4. Editor self-hosting via Atlas GUI DSL

---

## Test Coverage

All editor panel logic is covered by the existing test suite:

- `test_next_tasks.cpp` — StateHashDiffPanel, divergence, component breakdown
- `test_next_tasks_phase2.cpp` — SaveFileBrowserPanel, hash ladder, replay
- `test_replay_timeline_panel.cpp` — Timeline, markers, divergence callbacks
- `test_profiler_panel.cpp` — Frame timing, system metrics
- `test_atlas_assistant_panel.cpp` — Prompt, suggestions, conversation
- `test_ai_diff_viewer.cpp` — Hunk accept/reject, apply
- `test_ci_dashboard.cpp` — Pipeline runs, checks, history
- `test_game_mechanics_ui.cpp` — Elements, preview, export/import
