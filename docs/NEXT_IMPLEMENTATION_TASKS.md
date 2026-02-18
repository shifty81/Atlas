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
- [x] Complete ItchIOImporter implementation
  - API endpoint integration
  - Asset metadata parsing
  - Download and cache management
- [x] Complete UnrealMarketplaceImporter implementation
  - .uasset format parsing/conversion (binary header parsing with magic number validation)
  - API credential management
  - Mesh conversion pipeline
- [x] Complete UnityAssetStoreImporter implementation
  - .prefab format parsing (YAML key extraction)
  - API credential management
  - Material and shader translation
- [x] Add comprehensive tests for each importer
- [x] Document marketplace API setup in MARKETPLACE_IMPORTING.md
**Files modified**:
- `engine/assets/MarketplaceImporter.h` — Added ParseUAssetHeader, ParsePrefabHeader, SetApiCredential
- `engine/assets/MarketplaceImporter.cpp` — Real format conversion implementations
- `tests/test_next_tasks_phase11.cpp` — 8 marketplace tests

#### 2. Editor Truth UI Enhancements
**Status**: Core panels exist, missing real-time features

**Remaining work**:
- [x] Add live simulation state inspection
  - Real-time component value viewer
  - Entity hierarchy tree view
  - Component mutation tracking
- [x] Enhance Determinism View panel
  - Per-system hash breakdown
  - Frame-by-frame hash ladder
  - Visual diff of divergent components
- [x] Add Replay Inspector features
  - Input frame viewer
  - Event timeline visualization
  - Branch point markers
- [x] Implement tick-step debugging
  - Step forward/backward
  - Breakpoint on hash mismatch
  - State snapshot comparison

**Files modified**:
- `editor/panels/ECSInspectorPanel.h/.cpp` — Component value inspection, entity hierarchy, mutation tracking
- `editor/panels/StateHashDiffPanel.h/.cpp` — Per-system hash breakdown, hash ladder visualization
- `engine/sim/TickStepDebugger.h/.cpp` — New tick-step debugger with breakpoints

#### 3. CI Enhancement
**Status**: Basic determinism gate exists, needs tooling

**Remaining work**:
- [x] Implement replay minimizer
  - Automatically reduce failing replays to minimal reproduction
  - Binary search through replay frames
  - Output minimal .atlasreplay file
- [x] Add crash report bundling
  - Capture state snapshot on crash
  - Bundle with replay up to crash point
  - Generate hash ladder for debugging
- [x] Add cross-platform hash comparison dashboard
  - GitHub Action artifact summary
  - Visual diff of platform results
  - Automated bisection hints

**Files created**:
- `tools/replay_minimizer.py`
- `tools/crash_reporter.py`
- `.github/workflows/atlas_crash_reporter.yml`

### 📋 Planned (Lower Priority)

#### 4. CORE/ Namespace Enforcement
**Status**: Directory structure exists, CMake enforcement implemented

**Remaining work**:
- [x] Add CMake target dependency validation
  - Enforce that `core/` never depends on other modules
  - Enforce that `simulation/` only depends on `core/`
  - Enforce that `editor/` never links `simulation` directly
- [x] Add compile-time include guards
  - Prevent `simulation` from including `presentation` headers
  - Prevent `editor` from including `sim` internals
- [x] Document dependency graph in ARCHITECTURE.md

**Files created/modified**:
- `engine/core/contract/IncludeFirewall.h` - Compile-time include guards
- `docs/ARCHITECTURE.md` - Added layer enforcement documentation

#### 5. Advanced Debugging Tools
**Status**: Core features implemented

**Remaining work**:
- [x] State hash diff visualizer (interactive tool)
  - Component-by-component comparison
  - Per-system hash breakdown
  - Frame-by-frame hash ladder visualization
- [x] Tick-step debugger
  - Step forward/backward/jump
  - Tick and hash-mismatch breakpoints
  - Pause/resume with callbacks
- [x] Job execution tracer
  - Per-frame job execution order
  - Timing analysis
  - Determinism verification
- [x] Replay divergence inspector
  - Automatic divergence point detection
  - Component diff at divergence
  - Input replay from divergence
- [x] Standalone state diff viewer tool
  - `tools/state_diff_viewer.py` - Interactive CLI
- [x] Standalone replay inspector tool
  - `tools/replay_inspector.py` - Interactive CLI

**Files created**:
- `engine/sim/TickStepDebugger.h/.cpp` — Tick-step debugger with breakpoints
- `engine/render/VulkanRenderer.h/.cpp` — Sync primitives and memory pools

#### 6. Asset Pipeline Enhancements
**Status**: Basic pipeline exists, needs polish

**Remaining work**:
- [x] Hot-reload for marketplace assets
  - Detect asset updates from marketplace
  - Automatic re-import
  - Version conflict resolution
- [x] Asset validation dashboard
  - Integrity check results
  - Dependency graph visualization
  - Hash verification status
- [x] Mod asset sandboxing
  - Deterministic hash verification
  - Permission model enforcement
  - CPU/memory budget tracking

**Files created/modified**:
- `engine/assets/MarketplaceImporter.h` - Added MarketplaceHotReloader, AssetValidationDashboard, ModAssetSandbox
- `engine/assets/MarketplaceImporter.cpp` - Full implementations
- `editor/panels/ReplayTimelinePanel.h/.cpp` - Input frame viewer, event timeline, branch points
- `tools/state_diff_viewer.py` - Standalone state diff CLI
- `tools/replay_inspector.py` - Standalone replay inspector CLI
- `tests/test_next_tasks_phase12.cpp` - 25 new tests

#### 7. GL Viewport, Layout Resize, Bitmap Font
**Status**: Complete

**Completed work**:
- [x] GLViewportFramebuffer implementation
  - FBO with color texture and depth renderbuffer
  - Runtime GL extension resolution
  - Graceful fallback on platforms without GL
- [x] Editor layout resize propagation
  - `UIScreen::ScaleLayout` for proportional widget scaling
  - `UIManager::SetViewportSize` triggers layout scaling
  - `Engine::ProcessWindowEvents` propagates resize to UI
  - `Engine::InitEditor` sets initial viewport size
- [x] Bitmap font rendering in GLRenderer
  - Built-in 5×7 pixel font for printable ASCII (0x20–0x7E)
  - Replaces solid-rectangle placeholder text

**Files created/modified**:
- `engine/render/GLViewportFramebuffer.h` - GL FBO header
- `engine/render/GLViewportFramebuffer.cpp` - GL FBO implementation (conditional compilation)
- `engine/render/GLRenderer.cpp` - Bitmap font DrawText
- `engine/ui/UIScreenGraph.h/.cpp` - ScaleLayout method
- `engine/ui/UIManager.cpp` - SetViewportSize propagation
- `engine/core/Engine.cpp` - Resize and initial viewport wiring
- `tests/test_next_tasks_phase13.cpp` - 12 new tests

#### 8. Vulkan Device Abstraction, HttpLLMBackend, Font Discovery
**Status**: Complete

**Completed work**:
- [x] Vulkan hardware device abstraction
  - VkPhysicalDeviceInfo struct (device name, vendor, type, memory, capabilities)
  - VkQueueFamilyInfo struct (graphics, compute, transfer, present queues)
  - VkSwapChainDesc struct (image count, present mode, format)
  - VkDeviceConfig struct (application name, validation layers, extensions)
  - InitDevice/ShutdownDevice lifecycle with simulated GPU
  - Queue family discovery (graphics, compute, dedicated transfer)
  - Swap chain create/resize with validation
  - Device enumeration and selection
- [x] HttpLLMBackend for external LLM API integration
  - OpenAI-compatible chat completions API
  - API key management and authentication headers
  - JSON request body construction with proper escaping
  - JSON response parsing for content extraction
  - Error handling (no client, no key, HTTP error, parse error)
  - Success/failure tracking metrics
  - LLMBackendRegistry integration
  - Configurable timeout
- [x] FontBootstrap font discovery enhancements
  - Multiple font search path registration with deduplication
  - Font file discovery (.ttf, .otf) across all search paths
  - Direct font loading by path with TTF header validation
  - Loaded font path tracking
  - Init() auto-registers asset fonts directory

**Files created/modified**:
- `engine/render/VulkanRenderer.h` - VkPhysicalDeviceInfo, VkQueueFamilyInfo, VkSwapChainDesc, VkDeviceConfig, device management API
- `engine/render/VulkanRenderer.cpp` - Device initialization, queue discovery, swap chain management stubs
- `engine/ai/LLMBackend.h` - HttpLLMBackend class declaration
- `engine/ai/LLMBackend.cpp` - HttpLLMBackend implementation with JSON request/response handling
- `engine/ui/FontBootstrap.h` - Font search path and discovery API
- `engine/ui/FontBootstrap.cpp` - Font discovery and loading implementations
- `tests/test_next_tasks_phase14.cpp` - 35 new tests

#### 9. MeshViewerPanel and PrefabEditorPanel Draw Implementations
**Status**: Complete

**Completed work**:
- [x] MeshViewerPanel::Draw() implementation
  - Title bar with current view mode label (Solid, Wireframe, Solid+Wire, Normals)
  - Vertex/triangle count and bounding box display
  - Viewport area with border
  - Grid overlay (horizontal and vertical crosshair lines, togglable)
  - Normals overlay indicator when enabled
  - Selected vertex indicator
  - "No mesh loaded" fallback text
- [x] PrefabEditorPanel::Draw() implementation
  - Title bar with dirty indicator (*)
  - Entity hierarchy tree with recursive parent-child rendering
  - Indented child entities with depth-based offset
  - Parent nodes marked with > prefix
  - Component inspector section for selected entity
  - Component type display with property key-value pairs
  - "(no components)" fallback for entities without components
  - "Empty prefab" fallback text

**Files created/modified**:
- `editor/panels/MeshViewerPanel.h` - Added UIDrawList include, m_drawList member, GetDrawList() accessor, moved Draw() to .cpp
- `editor/panels/MeshViewerPanel.cpp` - New — full Draw() implementation with UIDrawList commands
- `editor/panels/PrefabEditorPanel.h` - Added UIDrawList include, m_drawList member, GetDrawList() accessor, DrawEntityRow() helper, moved Draw() to .cpp
- `editor/panels/PrefabEditorPanel.cpp` - New — full Draw() implementation with hierarchy tree and component inspector
- `tests/test_next_tasks_phase15.cpp` - 15 new tests

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

1. ~~Connect Vulkan hardware device — wire stub pipeline to real VkDevice/VkCommandBuffer (requires Vulkan SDK)~~ ✅ Conditional Vulkan SDK integration added (ATLAS_HAS_VULKAN_SDK)
2. ~~Ship real font — bundle Inter-Regular.ttf in builds~~ ✅ Inter-Regular.ttf bundled in assets/fonts/
3. ~~Deploy HttpLLMBackend — configure with production API endpoint and model~~ ✅ LLMBackendFactory added for env-based and explicit configuration

#### 10. Networking Improvements
**Status**: Complete

**Completed work**:
- [x] Latency/jitter simulation (GetSimulatedLatencyMs with deterministic jitter)
- [x] Bandwidth enforcement on Send/Broadcast (CanSendBytes gate, drop counting)
- [x] CRC32 packet checksum validation (compute on send, verify on receive)
- [x] Manual replication frequency (TriggerManualReplication for on-demand sync)
- [x] Reliable/unreliable delta split (CollectDelta for reliable, CollectUnreliableDelta for unreliable)
- [x] Hardening integration with NetContext (SetHardening, stats recording)
- [x] 23 new tests covering all networking improvements

**Files modified**:
- `engine/net/NetHardening.h/.cpp` — Added GetSimulatedLatencyMs()
- `engine/net/NetContext.h/.cpp` — Added SetHardening, ComputeChecksum, ValidateChecksum, bandwidth enforcement
- `engine/net/Replication.h/.cpp` — Added TriggerManualReplication, CollectUnreliableDelta, reliable/unreliable callbacks
- `tests/test_net_improvements.cpp` — 23 new tests

#### 11. AI Assistant LLM Wiring + Permission Enforcement
**Status**: Complete

**Completed work**:
- [x] EditorAssistant wired to LLMBackendRegistry
  - SetLLMBackend() / GetLLMBackend() methods
  - Unknown intents forwarded to LLM backend when available
  - Falls back to hardcoded responses when no LLM configured
  - Known intents (ExplainPerformance, ExplainGraphNode) unchanged
- [x] AssetGraphAssistant wired to LLMBackendRegistry
  - SetLLMBackend() / GetLLMBackend() methods
  - SuggestNodes() uses LLM for context-aware suggestions
  - ExplainGraph() uses LLM for richer explanations
  - MutateGraph() uses LLM for AI-powered mutation descriptions
  - All methods fall back to template responses when no LLM
- [x] EditorAttachProtocol permission enforcement
  - RequestOperation() validates connection state + permission tier + mode restrictions
  - Replay mode: read-only (blocks ModifyState, InjectInput, EditAssets, RunCI)
  - HeadlessServer mode: blocks StepSimulation
  - Permission tier check via IsOperationAllowed()
- [x] 21 new tests covering all changes

**Files modified**:
- `editor/assistant/EditorAssistant.h/.cpp` — Added LLM backend integration
- `editor/assistant/AssetGraphAssistant.h/.cpp` — Added LLM backend integration
- `editor/ui/EditorAttachProtocol.h/.cpp` — Added RequestOperation() with mode-aware enforcement
- `tests/test_next_tasks_phase16.cpp` — 21 new tests

#### 12. Remaining Panel Draw() Implementations
**Status**: Complete

**Completed work**:
- [x] AssetBrowserPanel Draw() implementation
  - Title bar, summary line with count/sort mode/filter
  - Asset list with extension display
  - Selected asset highlighting
- [x] NetInspectorPanel Draw() implementation
  - Title bar, network mode, peer count, connected count, average RTT
  - Peer list with per-peer status (connected/disconnected) and RTT
- [x] JobTracePanel inherits EditorPanel + Draw() implementation
  - Title bar, execution order status (consistent/mismatch)
  - Column header and trace entry rows
  - Mismatch highlighting in red
- [x] StateHashDiffPanel inherits EditorPanel + Draw() implementation
  - Title bar, divergence status summary
  - Column header and hash entry rows
  - Divergent entries highlighted in red
  - Per-system breakdown section when available
- [x] 26 new tests covering all 4 panel Draw() implementations

**Files modified**:
- `editor/panels/AssetBrowserPanel.h/.cpp` — Added UIDrawList member, GetDrawList(), full Draw() rendering
- `editor/panels/NetInspectorPanel.h/.cpp` — Added UIDrawList member, GetDrawList(), full Draw() rendering
- `editor/panels/JobTracePanel.h/.cpp` — Added EditorPanel inheritance, UIDrawList, Name(), Draw()
- `editor/panels/StateHashDiffPanel.h/.cpp` — Added EditorPanel inheritance, UIDrawList, Name(), Draw()
- `tests/test_panel_draw_impl.cpp` — 26 new tests

## References

- Original gaps analysis: `gaps.txt`
- Asset importing plan: `implement please`
- Project structure plan: `projectupdate`
- Core contract: `docs/ATLAS_CORE_CONTRACT.md`
- Determinism enforcement: `docs/ATLAS_DETERMINISM_ENFORCEMENT.md`
