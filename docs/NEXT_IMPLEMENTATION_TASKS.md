# Atlas Next Implementation Tasks

This document tracks the remaining implementation tasks to complete the vision outlined in the gaps analysis.

## Status: Current Implementation State

### ✅ Completed (High Priority)
- [x] ATLAS_CORE_CONTRACT.md documentation
- [x] Compile-time enforcement headers (AtlasContract.h, DeterministicRNG.h, SimulationGuard.h)
- [x] Runtime simulation guard implementation
- [x] CI determinism gate workflow
- [x] Contract violation bot
- [x] Contract scanner tool
- [x] Formal World State model (STATE_MODEL.md)
- [x] Time model specification (TIME_MODEL.md)
- [x] Asset importing foundation (AssetImporter.h, MarketplaceImporter.h)
- [x] Hash-based replay system
- [x] TLA+ formal specifications
- [x] Editor panel infrastructure (14+ panels)
- [x] State hash diff visualization
- [x] Replay timeline panel
- [x] ECS inspector
- [x] Network inspector

### 🚧 In Progress (Medium Priority)

#### 1. Marketplace Integration Completion
**Status**: Foundation exists, needs full implementation

**Remaining work**:
- [ ] Complete ItchIOImporter implementation
  - API endpoint integration
  - Asset metadata parsing
  - Download and cache management
- [ ] Complete UnrealMarketplaceImporter implementation
  - .uasset format parsing/conversion
  - Texture and material translation
  - Mesh conversion pipeline
- [ ] Complete UnityAssetStoreImporter implementation
  - .prefab format parsing
  - Unity to Atlas scene conversion
  - Material and shader translation
- [ ] Add comprehensive tests for each importer
- [ ] Document marketplace API setup in MARKETPLACE_IMPORTING.md

**Files to modify**:
- `engine/assets/MarketplaceImporter.cpp` (lines 100-300)
- Create test files in `tests/AssetImportTests.cpp`

#### 2. Editor Truth UI Enhancements
**Status**: Core panels exist, missing real-time features

**Remaining work**:
- [ ] Add live simulation state inspection
  - Real-time component value viewer
  - Entity hierarchy tree view
  - Component mutation tracking
- [ ] Enhance Determinism View panel
  - Per-system hash breakdown
  - Frame-by-frame hash ladder
  - Visual diff of divergent components
- [ ] Add Replay Inspector features
  - Input frame viewer
  - Event timeline visualization
  - Branch point markers
- [ ] Implement tick-step debugging
  - Step forward/backward
  - Breakpoint on hash mismatch
  - State snapshot comparison

**Files to modify**:
- `editor/panels/ECSInspectorPanel.cpp` - Add live inspection
- `editor/panels/StateHashDiffPanel.cpp` - Add component breakdown
- `editor/panels/ReplayTimelinePanel.cpp` - Add input frame viewer

#### 3. CI Enhancement
**Status**: Basic determinism gate exists, needs tooling

**Remaining work**:
- [ ] Implement replay minimizer
  - Automatically reduce failing replays to minimal reproduction
  - Binary search through replay frames
  - Output minimal .atlasreplay file
- [ ] Add crash report bundling
  - Capture state snapshot on crash
  - Bundle with replay up to crash point
  - Generate hash ladder for debugging
- [ ] Add cross-platform hash comparison dashboard
  - GitHub Action artifact summary
  - Visual diff of platform results
  - Automated bisection hints

**Files to create**:
- `tools/replay_minimizer.py`
- `tools/crash_reporter.py`
- `.github/workflows/atlas_crash_reporter.yml`

### 📋 Planned (Lower Priority)

#### 4. CORE/ Namespace Enforcement
**Status**: Directory structure exists, CMake enforcement missing

**Remaining work**:
- [ ] Add CMake target dependency validation
  - Enforce that `core/` never depends on other modules
  - Enforce that `simulation/` only depends on `core/`
  - Enforce that `editor/` never links `simulation` directly
- [ ] Add compile-time include guards
  - Prevent `simulation` from including `presentation` headers
  - Prevent `editor` from including `sim` internals
- [ ] Document dependency graph in ARCHITECTURE.md

**Files to modify**:
- `CMakeLists.txt` - Add dependency validation
- `engine/core/CMakeLists.txt` - Add include guards
- `docs/ARCHITECTURE.md` - Add dependency graph

#### 5. Advanced Debugging Tools
**Status**: Not started

**Remaining work**:
- [ ] State hash diff visualizer (interactive tool)
  - Component-by-component comparison
  - Visual highlighting of changes
  - Time-travel debugging interface
- [ ] Job execution tracer
  - Per-frame job execution order
  - Timing analysis
  - Determinism verification
- [ ] Replay divergence inspector
  - Automatic divergence point detection
  - Component diff at divergence
  - Input replay from divergence

**Files to create**:
- `tools/state_diff_viewer/` - New standalone tool
- `engine/sim/JobTracer.cpp` - Enhanced implementation
- `tools/replay_inspector/` - New standalone tool

#### 6. Asset Pipeline Enhancements
**Status**: Basic pipeline exists, needs polish

**Remaining work**:
- [ ] Hot-reload for marketplace assets
  - Detect asset updates from marketplace
  - Automatic re-import
  - Version conflict resolution
- [ ] Asset validation dashboard
  - Integrity check results
  - Dependency graph visualization
  - Hash verification status
- [ ] Mod asset sandboxing
  - Deterministic hash verification
  - Permission model enforcement
  - CPU/memory budget tracking

**Files to modify**:
- `engine/assets/MarketplaceImporter.cpp` - Add hot-reload
- `editor/panels/AssetBrowserPanel.cpp` - Add validation UI
- `engine/mod/ModLoader.cpp` - Add sandboxing

## Implementation Priority Order

1. **Marketplace Integration Completion** (Highest business value)
   - Enables asset ecosystem integration
   - Differentiates Atlas from other engines
   
2. **Editor Truth UI Enhancements** (Highest developer value)
   - Critical for debugging determinism issues
   - Improves developer experience significantly

3. **CI Enhancement** (Highest quality value)
   - Reduces debugging time
   - Catches issues earlier

4. **CORE/ Namespace Enforcement** (Architectural integrity)
   - Prevents technical debt
   - Enforces clean architecture

5. **Advanced Debugging Tools** (Long-term value)
   - Nice-to-have features
   - Can be added incrementally

6. **Asset Pipeline Enhancements** (Polish)
   - Quality-of-life improvements
   - Can be deferred

## Next Steps

1. Complete marketplace importers (itch.io, Unreal, Unity)
2. Add comprehensive tests for asset importing
3. Enhance Editor Truth UI with live inspection
4. Implement replay minimizer tool
5. Add CMake dependency enforcement

## References

- Original gaps analysis: `gaps.txt`
- Asset importing plan: `implement please`
- Project structure plan: `projectupdate`
- Core contract: `docs/ATLAS_CORE_CONTRACT.md`
- Determinism enforcement: `docs/ATLAS_DETERMINISM_ENFORCEMENT.md`
