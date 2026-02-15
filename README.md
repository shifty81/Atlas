# Atlas Game Engine

Atlas is a modular, data-driven game engine and simulation platform designed to support:

- **Standalone Editor** — Feature-rich, Blender-style authoring environment
- **Runtime Client** — Lean player runtime
- **Dedicated Server** — Headless authoritative simulation
- **Peer-to-Peer Networking** — Lockstep and rollback capable
- **Graph VM** — Deterministic, hot-reloadable gameplay logic
- **Massive Procedural Worlds** — Planet-scale to galaxy-scale generation
- **AI-Assisted Generation** — In-editor content and mechanic generation

## 🗺️ Development Roadmap

> **Project scope at a glance** — See [docs/09_DEVELOPMENT_ROADMAP.md](docs/09_DEVELOPMENT_ROADMAP.md) for full details.

```
  Phase 1        Phase 2        Phase 3        Phase 4        Phase 5        Phase 6        Phase 7
 Core Engine      Editor       Networking    World Gen       Gameplay      Production       Polish
────────────────────────────────────────────────────────────────────────────────────────────────────
 ██████████     ██████████     ██████████     ██████████     ░░░░░░░░░░     ██████░░░░     ░░░░░░░░░░
   100%            100%           100%          100%             0%            40%             0%
  ✅ Done        ✅ Done        ✅ Done       ✅ Done        📋 Planned    🔧 Active      📋 Planned
```

<table>
<tr>
<td width="14%">

**Phase 1 — Core Engine** ✅

- ✅ Bootstrap & config
- ✅ ECS framework
- ✅ Graph VM & compiler
- ✅ Asset registry
- ✅ Binary asset format
- ✅ Hot reload

</td>
<td width="14%">

**Phase 2 — Editor** ✅

- ✅ UI framework
- ✅ Panel docking system
- ✅ Graph editor panel
- ✅ ECS inspector
- ✅ Console panel
- ✅ Asset browser

</td>
<td width="14%">

**Phase 3 — Networking** ✅

- ✅ NetContext API
- ✅ Dedicated server
- ✅ P2P support
- ✅ Lockstep + rollback
- ✅ Replication rules

</td>
<td width="14%">

**Phase 4 — World Gen** ✅

- ✅ WorldLayout interface
- ✅ Cube-sphere math
- ✅ Voxel grid layout
- ✅ Terrain mesh gen
- ✅ Noise nodes
- ✅ World streaming
- ✅ Galaxy generation

</td>
<td width="14%">

**Phase 5 — Gameplay** 📋

- ⬜ Mechanic assets
- ⬜ Camera system
- ⬜ Input mapping
- ⬜ Physics integration
- ⬜ Audio system

</td>
<td width="14%">

**Phase 6 — Production** 🔧

- ✅ Game packager
- ✅ Asset cooker
- ⬜ Build profiles
- ⬜ Mod loader
- ⬜ Platform targeting

</td>
<td width="14%">

**Phase 7 — Polish** 📋

- ⬜ Undo/redo system
- ⬜ Visual diff tools
- ⬜ Profiler panels
- ⬜ Replay recorder
- ⬜ Crash analysis

</td>
</tr>
</table>

| Status | Meaning |
|--------|---------|
| ✅ Done | Phase fully complete |
| 🔧 Active | Currently in development |
| 📋 Planned | Not yet started |

## Architecture

Atlas ships as three executables, all linked against the same engine core:

| Binary         | Purpose                          |
|----------------|----------------------------------|
| `AtlasEditor`  | Full editor + runtime (superset) |
| `AtlasClient`  | Player runtime                   |
| `AtlasServer`  | Headless authoritative server    |

## Repository Structure

```
Atlas/
├── engine/              # Core engine library
│   ├── core/            # Engine bootstrap, logging, config
│   ├── ecs/             # Entity/Component/System
│   ├── graphvm/         # Deterministic Graph VM + compiler
│   ├── assets/          # Asset registry, binary format, hot reload
│   ├── net/             # Networking (CS + P2P + lockstep/rollback)
│   ├── sim/             # Tick scheduler, deterministic simulation
│   ├── world/           # World layouts (cube-sphere, voxel grid)
│   ├── math/            # Math utilities
│   ├── input/           # Input system
│   └── render/          # Render abstraction
│
├── editor/              # Standalone editor (Dev Client)
│   ├── ui/              # Docking, layout, panel system
│   ├── panels/          # ECS Inspector, Net Inspector, Console
│   ├── tools/           # Game Packager, Asset Cooker
│   ├── ai/              # AI Aggregator for asset generation
│   └── app/             # Editor application
│
├── client/              # Runtime game client
├── server/              # Headless server
├── shared/              # Shared utilities
├── assets/              # Game assets (.atlas, .atlasb)
├── tests/               # Unit tests
├── tools/               # Build tools and utilities
├── samples/             # Sample projects and demos
│
├── docs/                # Documentation
│   ├── 00_OVERVIEW.md
│   ├── 01_ENGINE_ARCHITECTURE.md
│   ├── 02_EDITOR_DESIGN.md
│   ├── 03_ASSET_SYSTEM.md
│   ├── 04_GRAPH_VM.md
│   ├── 05_NETWORKING.md
│   ├── 06_WORLD_GENERATION.md
│   ├── 07_GAMEPLAY_MECHANICS.md
│   ├── 08_AI_EDITOR_ASSIST.md
│   └── 09_DEVELOPMENT_ROADMAP.md
│
└── CMakeLists.txt
```

## Building

### Prerequisites

- CMake 3.22+
- C++20 compatible compiler (GCC 13+, Clang 15+, MSVC 2022+)

### Build

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### Run

```bash
# Editor (authoring + testing)
./editor/AtlasEditor

# Client (player runtime)
./client/AtlasClient

# Server (headless)
./server/AtlasServer
```

### Tests

```bash
cd build
ctest
# or
./tests/AtlasTests
```

## Core Design Principles

1. **Engine Is Not a Game** — The engine contains no gameplay logic
2. **Everything Is Data** — All behavior is authored as assets and executable graphs
3. **One Engine, Three Roles** — Editor, Client, and Server share the same core
4. **Editor Is Primary** — The editor is a first-class engine runtime
5. **Determinism First** — All simulation is deterministic for networking and replay

## Documentation

See the [docs/](docs/) directory for detailed architecture documentation.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.
