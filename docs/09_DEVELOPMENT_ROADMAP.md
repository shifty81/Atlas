# Atlas Engine — Development Roadmap

## Phase 1 — Core Engine

- [x] Engine bootstrap (config, logging, time)
- [x] ECS (entity/component/system)
- [x] Graph VM interpreter
- [x] Graph IR and compiler
- [x] Asset registry + hot reload
- [x] Binary asset format

## Phase 2 — Editor

- [x] UI framework (panels, docking)
- [x] Editor shell with default layout
- [x] Graph editor panel (stub)
- [x] ECS inspector panel (stub)
- [x] Console panel (stub)
- [x] Asset browser (stub)

## Phase 3 — Networking

- [x] NetContext API
- [x] Dedicated server loop
- [x] P2P support
- [x] Lockstep + rollback
- [x] Replication rules

## Phase 4 — World Generation

- [x] WorldLayout interface
- [x] Cube-sphere planet math
- [x] Voxel grid layout
- [x] Terrain mesh generation
- [x] Graph noise nodes (Perlin / FBM)
- [x] World streaming + disk cache
- [x] Galaxy generation

## Phase 5 — Gameplay

- [ ] Mechanic asset format
- [ ] Camera system (strategy, FPS, orbital)
- [ ] Input mapping system
- [ ] Physics integration
- [ ] Audio system

## Phase 6 — Production

- [x] Game packager tool (stub)
- [x] Asset cooker (stub)
- [ ] Build profiles
- [ ] Mod loader
- [ ] Platform targeting

## Phase 7 — Polish

- [ ] Undo/redo system
- [ ] Visual diff tools
- [ ] Profiler panels
- [ ] Replay recorder
- [ ] Crash analysis tools
