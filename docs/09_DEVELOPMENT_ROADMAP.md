# Atlas Engine — Development Roadmap

> This roadmap reflects the **actual implementation status** of each feature.
> Items marked as stubs have scaffolding code but lack functional logic.

---

## Phase 1 — Core Engine ✅

All core engine systems are implemented and tested.

- [x] Engine bootstrap (config, logging, lifecycle management)
- [x] ECS framework (entity creation, typed components, tick callbacks)
- [x] Graph VM interpreter (bytecode execution with full opcode dispatch)
- [x] Graph IR and compiler (topological sort, cycle detection, compilation)
- [x] Graph serialization (JSON builder/reader, no external dependencies)
- [x] Graph cache (tick-based invalidation, hash-keyed lookup)
- [x] Asset registry (filesystem scanning, type indexing)
- [x] Binary asset format (`.atlasb` with header, type, version)
- [x] Hot reload (file timestamp polling)
- [x] Schema validation system (versioned schemas, field integrity checks)
- [x] Tick scheduler (fixed-rate deterministic time stepping)

## Phase 2 — Editor 🔧

The editor framework is in place with docking and several functional panels.
Some panels are scaffolded but not yet feature-complete.

- [x] UI framework (EditorPanel base class, visibility control)
- [x] Panel docking system (recursive dock nodes, horizontal/vertical splits)
- [x] Editor layout manager (tree traversal rendering)
- [x] Console panel (command parsing, entity spawning, ECS dump, net queries)
- [x] World graph panel (node palette, canvas, inspector, profiler sections)
- [x] Project picker panel (project selection, recent projects list)
- [x] Voice command panel (voice processing and intent dispatch)
- [x] Interaction debugger panel (interaction logging and analysis)
- [x] Net inspector panel (network state, peers, RTT, bandwidth)
- [ ] ECS inspector panel (stub — `Draw()` exists but has no functional logic)
- [ ] Graph editor panel (stub — not yet implemented beyond scaffolding)
- [ ] Asset browser (not implemented)
- [ ] Editor assistant (interface exists, `Explain` methods are stubs)

## Phase 3 — Networking 🔧

Core networking API is implemented. Lockstep and rollback methods exist
but contain stub logic that needs to be completed.

- [x] NetContext API (mode selection, peer management, packet send/receive)
- [x] Dedicated server loop (headless mode, tick processing)
- [x] P2P host/peer support (peer add/remove, RTT tracking)
- [x] Loopback mode for testing
- [ ] Lockstep synchronization (method exists, ECS serialization is a stub)
- [ ] Rollback/replay (method exists, ECS restore is a stub)
- [ ] Replication rules (not implemented)

## Phase 4 — World Generation ✅

The most complete subsystem. Full procedural generation pipeline from
noise to terrain to galaxies, with streaming and disk caching.

- [x] WorldLayout interface (abstract coordinate mapping)
- [x] Cube-sphere planetary projection with LOD
- [x] Voxel grid chunked layout
- [x] Terrain mesh generation with normals
- [x] Graph noise nodes (Perlin, FBM — seeded, deterministic)
- [x] World streaming with disk cache
- [x] Galaxy generation (star systems, regions, spiral arms, star classes)
- [x] WorldGraph (DAG-based generation with typed values, topological compilation)
- [x] Heightfield mesher (heightfield → mesh pipeline with LOD)

## Phase 5 — Gameplay ✅

Core gameplay systems are implemented.

- [x] Mechanic asset format (MechanicAsset with parameter maps)
- [x] Skill tree system (nodes, connections, unlocking)
- [x] Camera system (world modes, projection policies)
- [x] Input mapping system (bindings, state tracking, callbacks)
- [x] Physics integration (rigid bodies, AABB collision detection, forces)
- [x] Audio system (sound loading, playback state management)

## Phase 5b — Graph Systems ✅

Atlas uses a DAG-based graph execution model across 14+ domains.
All graph types are implemented with compile/execute pipelines.

- [x] AnimationGraph (blend, pose, trigger, mask, modifier system)
- [x] CharacterGraph (modular character generation from seed)
- [x] WeaponGraph (weapon construction, wear degradation)
- [x] TileGraph (2D procedural tile maps)
- [x] SoundGraph (procedural audio synthesis)
- [x] StoryGraph (narrative generation and branching)
- [x] GameFlowGraph (boot → menu → gameplay → credits)
- [x] UIGraph + UIScreenGraph (UI composition)
- [x] StrategyGraph (influence, threat, scoring decisions)
- [x] ConversationGraph (dialogue flow, memory, relationships)
- [x] BehaviorGraph (AI behavior authoring)
- [x] AssetGraph (asset pipeline execution)

## Phase 5c — AI Systems ✅

AI subsystems with memory, signals, relationships, and behavior authoring.

- [x] AISignalRegistry (namespaced numeric inputs with tick tracking)
- [x] AIMemory (key-value storage with tick-based decay)
- [x] RelationshipModel (symmetric entity pair tracking)
- [x] FactionRouter (faction-based routing)
- [x] BehaviorGraph (authorable AI behavior as content)

## Phase 5d — Interaction & Voice ✅

Unified intent pipeline for console, voice, and AI inputs.

- [x] Utterance/Intent system (raw text → resolved action)
- [x] PatternIntentResolver (case-insensitive phrase matching)
- [x] IntentRegistry (central dispatch for handlers)
- [x] VoiceCommandRegistry (context-aware phrase → intent matching)
- [x] InteractionSystem (unified pipeline: Voice → Text → Intent → Command)

## Phase 5e — Project & Plugin System ✅

Multi-project architecture with schema validation and plugin support.

- [x] Project loading and validation (`.atlas` project files)
- [x] Schema-versioned project format
- [x] Plugin system (descriptor, validator, registry)
- [x] Mod asset registry (basic registration)
- [x] Server rules system (live parameter tuning with bounds)

## Phase 6 — Production 🔧

Production tooling is in early stages. The packager panel exists but
lacks functional build logic.

- [x] Game packager panel (UI scaffolding for build targets and modes)
- [ ] Asset cooker (not implemented)
- [ ] Build profiles (Debug/Development/Release — UI exists, logic missing)
- [ ] Mod loader (ModAssetRegistry exists but minimal)
- [ ] Platform targeting (not implemented)

## Phase 7 — Polish 🔧

Some polish items have been implemented earlier than planned.

- [x] Undo/redo system (CommandHistory with execute/undo/redo stacks)
- [ ] Visual diff tools
- [ ] Profiler panels
- [ ] Replay recorder
- [ ] Crash analysis tools

---

## Implementation Summary

| Area | Status | Notes |
|------|--------|-------|
| Core Engine | ✅ Complete | ECS, Graph VM, assets, simulation |
| Graph Systems (14 types) | ✅ Complete | All graph types implemented with DAG execution |
| World Generation | ✅ Complete | Full pipeline: noise → terrain → galaxy → streaming |
| AI Systems | ✅ Complete | Memory, signals, relationships, behavior graphs |
| Gameplay | ✅ Complete | Camera, input, physics, audio, mechanics |
| Interaction/Voice | ✅ Complete | Unified intent pipeline |
| Project/Plugin | ✅ Complete | Multi-project, schema validation, plugins |
| Editor Framework | 🔧 Partial | Docking and several panels work; some stubs remain |
| Networking | 🔧 Partial | API works; lockstep/rollback are stubs |
| Production Tools | 🔧 Early | Packager UI scaffolded; no build logic |
| Polish | 🔧 Early | Undo/redo done; other items not started |
