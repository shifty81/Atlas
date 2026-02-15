# Atlas Procedural Modeling System

## Overview

The Procedural Modeling System is a Blender-like modeling workspace fully
integrated into the Atlas editor. All modeling operations are deterministic and
graph-based, producing meshes, materials, shaders, animations, and LODs that are
replayable, diffable, and CI-testable.

Core principles:

- Every modeling operation is a node in a procedural graph
- All output is deterministic — identical graphs always produce identical assets
- The modeling workspace is an editor panel, not an external tool
- Procedural graphs compile through the same asset pipeline as hand-authored assets
- All generated assets are hash-addressable and version-controlled

## Modeling Workspace UI

The workspace follows the same panel architecture described in
[13_EDITOR_UI.md](13_EDITOR_UI.md) and integrates directly into the editor shell.

```
ProceduralModelingWorkspace
 ├── Viewport          — 3D preview with manipulation gizmos
 ├── Toolbar           — sculpt, extrude, bevel, smooth (all via graph nodes)
 ├── NodeGraph         — procedural mesh definition as a node graph
 ├── Properties        — material, UV, normals, physics properties
 └── LODPreview        — multi-resolution display
```

| Panel        | Purpose                                              |
|--------------|------------------------------------------------------|
| Viewport     | Real-time 3D preview rendered via the engine renderer |
| Toolbar      | Quick-access modeling operations mapped to graph nodes |
| Node Graph   | Visual authoring of procedural mesh graphs            |
| Properties   | Per-node and per-mesh property inspector              |
| LOD Preview  | Side-by-side LOD comparison with triangle counts      |

## Procedural Mesh Graph Library

Procedural meshes are defined as directed acyclic graphs of typed nodes.
Node graphs compile to Graph VM bytecode (see [04_GRAPH_VM.md](04_GRAPH_VM.md))
and follow the same IR rules as flow graphs
(see [15_FLOW_GRAPH.md](15_FLOW_GRAPH.md)).

### Node Categories

| Category    | Nodes                                          |
|-------------|------------------------------------------------|
| Primitives  | cube, sphere, cylinder, plane, torus           |
| Operations  | extrude, bevel, subdivide, mirror, boolean     |
| Deformers   | bend, twist, taper, lattice, noise             |
| Utilities   | merge, split, duplicate, instance              |
| Constraints | symmetry, alignment, snapping                  |

All nodes are pure or explicitly side-effecting, and the compiler enforces this
classification at build time — identical to the flow graph guarantee.

## Material & Shader Graphs

- Procedural material authoring via node graph
- PBR material model: albedo, metallic, roughness, normal, AO
- Custom shader graph compiles to AtlasShaderIR
- All materials are deterministic — identical graphs produce identical shaders
- Hot-reloadable in editor with instant viewport feedback

```
MaterialGraph
 ├── TextureInputs     — procedural or sampled textures
 ├── PBRCombiner       — albedo, metallic, roughness, normal, AO
 ├── ShaderCompiler    → AtlasShaderIR
 └── Preview           — real-time material preview in viewport
```

## LOD & Mesh Baking

LOD graphs define the decimation strategy per level. All baking operations run
through the graph system to guarantee deterministic output.

| Operation    | Description                                         |
|--------------|-----------------------------------------------------|
| Decimation   | Graph-defined triangle reduction per LOD level       |
| Normal bake  | High-poly to low-poly normal map transfer            |
| AO bake      | Ambient occlusion from mesh geometry                 |
| Displacement | Displacement map extraction from high-poly detail    |

- Deterministic mesh simplification — identical input always produces identical LODs
- LOD selection thresholds are authored in the graph, not hardcoded
- Baked outputs are hash-addressed and cached by the asset pipeline

## Deterministic Animation Graphs

Procedural animation is defined as node graphs that compile to deterministic,
tick-reproducible bytecode.

- Bone and skeleton manipulation nodes
- IK/FK solver nodes (deterministic, fixed-point where required)
- Blend tree with fixed-point interpolation
- Animation state machines authored graphically
- All animation is tick-deterministic — identical inputs produce identical poses

```
AnimationGraph
 ├── SkeletonInput     — bone hierarchy definition
 ├── IK/FK Solvers     — deterministic inverse/forward kinematics
 ├── BlendTree         — fixed-point interpolation nodes
 ├── StateMachine      — transition logic authored as a graph
 └── PoseOutput        — final skeletal pose per tick
```

## Collaborative Editing

The modeling workspace supports networked collaborative sessions, following
the same editor networking model used elsewhere in Atlas.

- Live cursors for multi-user editing within the same graph
- Conflict resolution for concurrent graph modifications
- Merge strategy for procedural assets based on node-level diffing
- Session state is authoritative — no silent overwrites

## Deterministic Mesh Format

Procedural meshes serialize to a custom binary format designed for determinism
and version control.

| Property               | Guarantee                                       |
|------------------------|-------------------------------------------------|
| Format                 | Custom binary, platform-independent              |
| Deterministic          | Identical graphs always produce identical bytes   |
| Hash-addressable       | Every mesh is content-addressed for verification  |
| Schema-versioned       | `format_version` field enables automatic migration |
| Diff-friendly          | Structured layout supports meaningful binary diffs |

## Integration with Engine

- Procedural assets feed into the same asset pipeline as hand-authored assets
- Editor preview renders via the engine renderer — no separate preview path
- CI validates deterministic output of procedural graphs on every commit
- Generated meshes are hashed like any other asset
  (see [03_ASSET_SYSTEM.md](03_ASSET_SYSTEM.md))

## File Locations

| Path                 | Purpose                          |
|----------------------|----------------------------------|
| `engine/character/`  | Character procedural generation  |
| `engine/tile/`       | Tile and tileset generation      |
| `engine/weapon/`     | Weapon procedural generation     |
| `engine/animation/`  | Animation graph system           |
