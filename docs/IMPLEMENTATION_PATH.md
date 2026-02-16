# Atlas Engine — Implementation Path

> This document consolidates all identified gaps across the engine and defines
> a concrete, prioritized implementation path. Each item references the source
> file or system it belongs to and its current status.
>
> **Audiences:** Engine developers, contributors, project leads.
>
> See also: `docs/09_DEVELOPMENT_ROADMAP.md`, `docs/ATLAS_LOCKDOWN_CHECKLIST.md`

---

## Status Legend

| Symbol | Meaning |
|--------|---------|
| ✅ | Implemented and tested |
| 🔧 | Partially implemented or needs hardening |
| ❌ | Not yet implemented |

---

## Priority 1 — Determinism Foundation (Critical Path)

These items are prerequisites for network correctness, replay fidelity,
and save/load integrity. They must be completed before any content-facing
milestone ships.

### 1.1 Time Model ✅

**Files:** `engine/sim/TimeModel.h`, `engine/sim/TimeModel.cpp`
**Tests:** `tests/test_time_model.cpp` (8 tests)

Defines three canonical time layers:

| Layer | Purpose | Enters Simulation? |
|-------|---------|-------------------|
| `SimulationTime` | Fixed-step tick counter | Yes (authoritative) |
| `WorldTime` | In-universe elapsed time with dilation | Yes (read-only in systems) |
| `PresentationTime` | Wall-clock interpolation for rendering | **No** |

**Remaining work:**
- [ ] Wire `TimeModel` into `Engine` to replace raw `TickScheduler` delta calls
- [ ] Expose `TimeContext` to ECS system callbacks
- [ ] Add `WorldTime` dilation controls to editor HUD overlay

### 1.2 Formal World State Model ✅

**Files:** `engine/sim/WorldState.h`, `engine/sim/WorldState.cpp`
**Tests:** `tests/test_world_state.cpp` (9 tests)

Defines state categories:

| Category | Hashed? | Serialized? | Replicated? |
|----------|---------|-------------|-------------|
| `Simulated` | Yes | Yes | Yes |
| `Derived` | No | No | No |
| `Presentation` | No | No | No |
| `Debug` | No | No | No |

Provides snapshot/rollback support with configurable history depth.

**Remaining work:**
- [ ] Annotate every ECS component with its `StateCategory`
- [ ] Enforce category at compile-time via type traits or tag types
- [ ] Add snapshot integration to lockstep rollback path in `NetContext`
- [ ] Expose state block inspector in editor ECS panel

### 1.3 Save / Load System ✅

**Files:** `engine/sim/SaveSystem.h`, `engine/sim/SaveSystem.cpp`
**Tests:** `tests/test_save_system.cpp` (7 tests)

Provides versioned binary save format (`.asav`) with hash-verified integrity.

**Remaining work:**
- [ ] Add save/load commands to editor console panel
- [ ] Wire into `Engine::RunClient()` and `Engine::RunServer()` for autosave
- [ ] Implement partial-world saves for large streaming worlds
- [ ] Add save file browser to editor UI
- [ ] Cross-platform save compatibility testing
- [ ] Replay-from-save verification (load save → replay → verify hash ladder)

### 1.4 Determinism Enforcement Hardening 🔧

**Files:** `engine/core/contract/AtlasContract.h`, `SimulationGuard.h`,
          `DeterministicRNG.h`, `DeterministicAllocator.h`

Compile-time guards and runtime assertions exist. Gaps remain:

- [ ] Add `ATLAS_FORBID_IN_SIM` macro to prevent presentation code in tick
- [ ] Enforce `#include` firewall: simulation code cannot include render headers
- [ ] Add CMake `INTERFACE` target separating sim-safe from render-safe sources
- [ ] FP consistency enforcement (`-ffp-contract=off`, `/fp:strict` flags)
- [ ] Platform-dependent math detection in CI (scan for `__m128` outside render)

### 1.5 AI Determinism Contract ❌

AI systems must produce bit-identical decisions given identical inputs.

- [ ] Document allowed randomness sources in AI systems (DeterministicRNG only)
- [ ] Serialize planner state for save/load (BehaviorGraph execution state)
- [ ] Add determinism test: same AIMemory + inputs → same BehaviorGraph output
- [ ] Verify RelationshipModel determinism across save/load boundary

---

## Priority 2 — Engine Integration & Wiring

These items connect the new foundational systems to the existing engine
runtime and editor infrastructure.

### 2.1 Engine Runtime Integration 🔧

- [ ] Integrate `TimeModel` into `Engine` class (replace direct `TickScheduler` usage)
- [ ] Integrate `WorldState` snapshotting into `Engine::RunServer()` loop
- [ ] Integrate `SaveSystem` save/load into `Engine` lifecycle
- [ ] Pass `TimeContext` through ECS `Update()` instead of raw `float dt`

### 2.2 Networking ↔ State Integration 🔧

- [ ] Use `WorldSnapshot` in lockstep sync instead of raw ECS serialization
- [ ] Snapshot-based rollback: restore from `WorldState` snapshot + replay
- [ ] Network save/load: server broadcasts save tick, clients verify hash

### 2.3 Replay ↔ Save Integration ❌

- [ ] Verify hash ladder continuity across save/load boundaries
- [ ] Implement "replay from save" mode (load save → begin replay playback)
- [ ] Add save-point markers in replay files

---

## Priority 3 — Tooling & Debug Infrastructure

### 3.1 State Hash Diff Visualizer ❌

- [ ] Editor panel showing per-component hash breakdown at any tick
- [ ] Side-by-side comparison of two hash ladders
- [ ] Highlight first divergence point with component-level detail

### 3.2 Replay Divergence Inspector UI 🔧

**Files:** `engine/sim/ReplayDivergenceInspector.h/cpp`,
          `editor/panels/ReplayTimelinePanel.h/cpp`

Core logic exists. Remaining:
- [ ] Auto-open divergence view on hash mismatch during replay
- [ ] Show per-system state diff at divergence tick
- [ ] Export divergence report to file

### 3.3 FP Drift Detector ❌

- [ ] Tool to run identical simulation on two platforms and compare hashes
- [ ] CI job that cross-compiles and verifies golden replay hashes
- [ ] Report showing which systems first diverge

### 3.4 Job Execution Trace Viewer ❌

- [ ] Record system execution order each tick
- [ ] Editor panel visualizing tick timeline (which system ran when)
- [ ] Detect non-deterministic execution ordering

---

## Priority 4 — Documentation Gaps

All top-level documentation files exist. The following need content
hardening to match implementation reality:

### 4.1 Documentation Updates Needed

| Document | Status | Action |
|----------|--------|--------|
| `05_NETWORKING.md` | 🔧 Exists | Add lockstep frame semantics, rollback window details |
| `07_GAMEPLAY_MECHANICS.md` | 🔧 Exists | Add simulation-first rules, forbidden APIs list |
| `03_ASSET_SYSTEM.md` | 🔧 Exists | Add AssetCooker pipeline, binary format details |
| `06_WORLD_GENERATION.md` | 🔧 Exists | Add WorldGraph node types, streaming protocol |
| `BUILDING.md` | 🔧 Exists | Add platform-specific troubleshooting |
| `ARCHITECTURE.md` | 🔧 Exists | Add module dependency diagram |

### 4.2 New Documentation Needed

- [ ] `docs/TIME_MODEL.md` — Canonical time layer definitions and rules
- [ ] `docs/SAVE_SYSTEM.md` — Save format specification, compatibility policy
- [ ] `docs/STATE_MODEL.md` — State category rules, mutation ownership

---

## Priority 5 — Asset System Expansion

### 5.1 Asset Import Pipeline ❌

- [ ] FBX/OBJ/glTF mesh importer → internal mesh format
- [ ] Texture importer with compression (PNG, DDS, TGA → internal)
- [ ] Audio importer (WAV, OGG, FLAC → internal)
- [ ] Font importer (TTF, OTF → glyph atlas)
- [ ] Sprite sheet importer with animation sequence extraction

### 5.2 Asset Validation ❌

- [ ] Hash-based immutability enforcement at API level
- [ ] Asset version migration system (schema v1 → v2 auto-upgrade)
- [ ] Asset dependency graph visualization in editor

### 5.3 Asset Editor Panels ❌

- [ ] Mesh viewer panel (3D preview with wireframe toggle)
- [ ] Material editor panel (shader parameter editing)
- [ ] Prefab editor panel (drag-and-drop entity composition)

---

## Priority 6 — Advanced Systems (Post-Lockdown)

These are planned for future phases and do not block engine lockdown.

### 6.1 Flow Graph Runtime ❌

- [ ] Flow Graph IR file format and schema
- [ ] Flow Graph VM (deterministic node execution)
- [ ] Flow Graph debugger (breakpoints, value inspection)
- [ ] Flow Graph → C++ codegen for release builds

### 6.2 Procedural Modeling ❌

- [ ] Mesh graph primitives (cube, sphere, cylinder, plane)
- [ ] Mesh operations (boolean, bevel, subdivide, extrude)
- [ ] Material and shader graphs
- [ ] LOD and mesh baking pipeline

### 6.3 AtlasAI Assistant ❌

- [ ] Structured I/O with permission-gated intents
- [ ] Context-aware editor suggestions
- [ ] AI diff viewer (preview changes before applying)
- [ ] Web aggregation knowledge base

### 6.4 Template Repository System ❌

- [ ] `atlas init` CLI tool (namespace rewrite, core freeze)
- [ ] Template contract (ATLAS_TEMPLATE_CONTRACT.md)
- [ ] Determinism versioning strategy across forks

---

## Implementation Order

The recommended implementation sequence, reflecting dependency chains:

```
Phase A — Foundation (current sprint)
  ✅ TimeModel
  ✅ WorldState
  ✅ SaveSystem
  → Wire into Engine
  → Wire into Networking

Phase B — Enforcement
  → Determinism enforcement hardening
  → AI determinism contract
  → FP consistency flags
  → Include firewall (sim vs render)

Phase C — Integration
  → Engine runtime integration
  → Networking ↔ state integration
  → Replay ↔ save integration
  → Editor console commands for save/load

Phase D — Tooling
  → State hash diff visualizer
  → Replay divergence inspector UI
  → FP drift detector
  → Job execution trace viewer

Phase E — Documentation
  → Harden existing docs to match implementation
  → Write new docs (TIME_MODEL, SAVE_SYSTEM, STATE_MODEL)

Phase F — Assets
  → Mesh/texture/audio importers
  → Asset validation and migration
  → Editor asset panels

Phase G — Advanced (post-lockdown)
  → Flow Graph runtime
  → Procedural modeling
  → AtlasAI assistant
  → Template system
```

---

## Lockdown Gate

Atlas reaches **feature-locked** status when all Priority 1 and Priority 2
items are complete and verified per `docs/ATLAS_LOCKDOWN_CHECKLIST.md`.

After lockdown:
- Feature additions require contract review
- Determinism bugs are P0 severity
- All changes must pass golden replay CI gate
