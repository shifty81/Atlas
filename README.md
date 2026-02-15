```
╔══════════════════════════════════════════════════════════════════╗
║                                                                  ║
║       █████╗ ████████╗██╗      █████╗ ███████╗                   ║
║      ██╔══██╗╚══██╔══╝██║     ██╔══██╗██╔════╝                   ║
║      ███████║   ██║   ██║     ███████║███████╗                   ║
║      ██╔══██║   ██║   ██║     ██╔══██║╚════██║                   ║
║      ██║  ██║   ██║   ███████╗██║  ██║███████║                   ║
║      ╚═╝  ╚═╝   ╚═╝   ╚══════╝╚═╝  ╚═╝╚══════╝                   ║
║                                                                  ║
║          ⚙️  Modular · Data-Driven · Deterministic  ⚙️           ║
║                   G A M E   E N G I N E                          ║
║                                                                  ║
╚══════════════════════════════════════════════════════════════════╝
```

# Atlas Game Engine

Atlas is a modular, data-driven game engine and simulation platform built in C++20. All gameplay is authored as data and executable graphs — the engine contains no hardcoded gameplay logic.

### Key Features

- **Standalone Editor** — Blender-style authoring environment with dockable panels
- **Runtime Client & Server** — Lean player runtime and headless authoritative server
- **Graph VM** — Deterministic bytecode virtual machine for hot-reloadable gameplay logic
- **Graph-Based Systems** — 14+ graph types for world gen, animation, AI, audio, UI, and more
- **Procedural Generation** — Planet-scale terrain, galaxies, tiles, weapons, and characters
- **AI Systems** — Behavior graphs, memory with decay, faction relationships, strategy decisions
- **Networking** — Client-server and P2P with lockstep/rollback foundations
- **Project System** — Multi-project support with schema-validated `.atlas` project files

---

## 🗺️ Development Roadmap

> See [docs/09_DEVELOPMENT_ROADMAP.md](docs/09_DEVELOPMENT_ROADMAP.md) for full details.

```
  Phase 1        Phase 2        Phase 3        Phase 4        Phase 5        Phase 6        Phase 7
 Core Engine      Editor       Networking    World Gen       Gameplay      Production       Polish
────────────────────────────────────────────────────────────────────────────────────────────────────
 ██████████     ██████████     ██████████     ██████████     ██████████     ██████████     ██████████
   100%           100%           100%           100%           100%           100%           100%
  ✅ Done        ✅ Done        ✅ Done        ✅ Done        ✅ Done        ✅ Done        ✅ Done
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
- ✅ Console panel
- ✅ World graph panel
- ✅ ECS inspector
- ✅ Graph editor
- ✅ Asset browser

</td>
<td width="14%">

**Phase 3 — Networking** ✅

- ✅ NetContext API
- ✅ Dedicated server loop
- ✅ P2P support
- ✅ Lockstep sync
- ✅ Rollback/replay
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

**Phase 5 — Gameplay** ✅

- ✅ Mechanic assets
- ✅ Camera system
- ✅ Input mapping
- ✅ Physics integration
- ✅ Audio system

</td>
<td width="14%">

**Phase 6 — Production** ✅

- ✅ Game packager
- ✅ Asset cooker
- ✅ Build profiles
- ✅ Mod loader
- ✅ Platform targeting

</td>
<td width="14%">

**Phase 7 — Polish** ✅

- ✅ Undo/redo system
- ✅ Visual diff tools
- ✅ Profiler panel
- ✅ Replay recorder
- ✅ Crash analysis

</td>
</tr>
</table>

| Status | Meaning |
|--------|---------|
| ✅ Done | Phase fully implemented |

---

## Architecture

Atlas ships as four executables, all linked against the same engine core:

| Binary          | Purpose                                       |
|-----------------|-----------------------------------------------|
| `AtlasEditor`   | Full editor + runtime (superset)              |
| `AtlasRuntime`  | Standalone runtime with CLI project loading   |
| `AtlasClient`   | Player runtime                                |
| `AtlasServer`   | Headless authoritative server                 |

## Repo Split Plan

For keeping server/client/editor generic while hosting game code in external modules (e.g., EveOffline), see [docs/10_REPO_SPLIT_PLAN.md](docs/10_REPO_SPLIT_PLAN.md) for the concrete layout, module interface, and migration steps.

## Repository Structure

```
Atlas/
├── engine/              # Core engine static library
│   ├── core/            # Engine bootstrap, logging, config
│   ├── ecs/             # Entity-Component-System framework
│   ├── graphvm/         # Deterministic Graph VM + compiler + serialization + cache
│   ├── assets/          # Asset registry, binary format, hot reload
│   ├── net/             # Networking (client-server + P2P)
│   ├── sim/             # Tick scheduler, deterministic simulation, replay recorder
│   ├── world/           # World generation (cube-sphere, voxel, terrain, galaxy, streaming)
│   ├── input/           # Input mapping system
│   ├── camera/          # Camera system with world mode policies
│   ├── physics/         # Physics simulation (rigid bodies, AABB collision)
│   ├── audio/           # Audio engine
│   ├── gameplay/        # Mechanic assets & skill trees
│   ├── ai/              # AI signals, memory, relationships, behavior graphs
│   ├── interaction/     # Intent/utterance system (voice, AI, console)
│   ├── voice/           # Voice command registry and matching
│   ├── conversation/    # Dialogue and memory graphs
│   ├── strategygraph/   # Strategy decision graphs (influence, threat, scoring)
│   ├── animation/       # Animation graph + modifier system
│   ├── character/       # Character generation graph
│   ├── weapon/          # Weapon construction graph
│   ├── tile/            # 2D tileset generation graph
│   ├── sound/           # Procedural audio/synth graph
│   ├── story/           # Story/narrative graph
│   ├── flow/            # Game flow graph (boot → credits)
│   ├── ui/              # UI composition graph
│   ├── asset_graph/     # Asset graph executor
│   ├── command/         # Undo/redo command history
│   ├── project/         # Project loading and validation
│   ├── schema/          # Schema validation system
│   ├── plugin/          # Plugin system (validation, registry)
│   ├── mod/             # Mod asset registry and mod loader
│   ├── module/          # Game module interface and dynamic loader
│   ├── production/      # Asset cooker, build profiles, platform targeting
│   └── rules/           # Server rules (live parameter tuning)
│
├── modules/
│   └── atlas_gameplay/  # AtlasGameplay static lib (factions, combat, economy)
│
├── editor/              # Standalone editor application
│   ├── ui/              # Docking, layout, panel framework
│   ├── panels/          # Console, ECS Inspector, Net Inspector, World Graph,
│   │                    # Project Picker, Voice Commands, Interaction Debugger, Profiler
│   ├── tools/           # Game packager panel, visual diff tools
│   ├── ai/              # AI aggregator for asset generation
│   └── assistant/       # Editor assistant (explain, suggest)
│
├── runtime/             # Standalone runtime (CLI project loading)
├── client/              # Player runtime client
├── server/              # Headless dedicated server
│
├── tests/               # Unit tests (90 test files)
│
├── schemas/             # Versioned JSON schemas
│   ├── atlas.project.v1.json
│   ├── atlas.worldgraph.v1.json
│   ├── atlas.strategygraph.v1.json
│   └── atlas.conversation.v1.json
│
├── projects/            # Sample game projects
│   ├── eveoffline/      # Space strategy reference project
│   │   └── module/      # EveOfflineModule (IGameModule implementation)
│   ├── arena2d/         # 2D arena reference project
│   └── atlas-sample/    # Minimal sample project
│
├── docs/                # Documentation
├── logs/                # Build and runtime logs (gitignored contents)
└── CMakeLists.txt       # Root build configuration
```

## Building

### Prerequisites

- CMake 3.22+
- C++20 compatible compiler (GCC 13+, Clang 15+, MSVC 2022+)

### Build

The easiest way to build is with the included build script:

```bash
# Build all targets (server, client, editor, runtime) in Release mode
./build.sh

# Build specific targets
./build.sh server client          # Server and client only
./build.sh editor                 # Developer client (editor) only
./build.sh engine                 # Engine and gameplay libraries only

# Build options
./build.sh -b Debug all           # Debug build
./build.sh -b Development editor  # Development build (optimized + debug symbols)
./build.sh --clean --test all     # Clean rebuild with tests
./build.sh -o ./my-output server  # Custom output directory
./build.sh --install              # Install SDK (headers + libs + cmake configs) to dist/sdk/

# See all options
./build.sh --help
```

Executables are placed in `dist/` by default. SDK artifacts (for external game modules) are installed to `dist/sdk/` with the `--install` flag.

You can also build manually with CMake:

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### Run

```bash
# Editor / developer client (authoring + testing)
./dist/AtlasEditor

# Runtime (standalone with project loading)
./dist/AtlasRuntime --project projects/atlas-sample/sample.atlas

# Client (player runtime)
./dist/AtlasClient

# Server (headless)
./dist/AtlasServer
```

### Tests

```bash
# Via build script
./build.sh --test

# Or manually
cd build
ctest
# or
./tests/AtlasTests
```

---

## Core Design Principles

1. **Engine Is Not a Game** — The engine contains no gameplay logic; all behavior is data
2. **Everything Is Data** — All behavior is authored as assets and executable graphs
3. **One Engine, Four Roles** — Editor, Runtime, Client, and Server share the same core
4. **Editor Is Primary** — The editor is a first-class engine runtime, not a separate tool
5. **Determinism First** — All simulation is deterministic for networking and replay
6. **Graphs Everywhere** — 14+ domain-specific graph types power world gen, AI, animation, audio, and more

## Graph Systems

Atlas uses a DAG-based graph execution model across many domains:

| Graph Type | Purpose |
|------------|---------|
| WorldGraph | Procedural terrain and world generation |
| StrategyGraph | AI strategy decisions (influence, threat) |
| ConversationGraph | Dialogue and NPC memory |
| BehaviorGraph | Authorable AI behavior trees |
| AnimationGraph | Animation state machines with modifiers |
| CharacterGraph | Modular character generation |
| WeaponGraph | Weapon construction and wear |
| TileGraph | 2D procedural tile maps |
| SoundGraph | Procedural audio synthesis |
| UIGraph | UI composition (panels, buttons, layouts) |
| UIScreenGraph | Screen-level UI management |
| GameFlowGraph | Game state flow (boot → credits) |
| StoryGraph | Narrative generation and branching |
| AssetGraph | Asset pipeline execution |

## Documentation

See the [docs/](docs/) directory for detailed documentation:

| Document | Description |
|----------|-------------|
| [Overview](docs/00_OVERVIEW.md) | High-level engine capabilities and philosophy |
| [Engine Architecture](docs/01_ENGINE_ARCHITECTURE.md) | Module structure, boot flow, runtime modes |
| [Editor Design](docs/02_EDITOR_DESIGN.md) | Editor panels, docking, play-in-editor modes |
| [Asset System](docs/03_ASSET_SYSTEM.md) | Asset types, binary format, hot reload |
| [Graph VM](docs/04_GRAPH_VM.md) | Bytecode VM, instruction set, compilation |
| [Networking](docs/05_NETWORKING.md) | NetContext, replication, lockstep/rollback |
| [World Generation](docs/06_WORLD_GENERATION.md) | Terrain, planets, galaxies, streaming |
| [Gameplay Mechanics](docs/07_GAMEPLAY_MECHANICS.md) | Mechanic assets, skill trees, cameras |
| [AI Editor Assist](docs/08_AI_EDITOR_ASSIST.md) | AI-assisted content generation |
| [Development Roadmap](docs/09_DEVELOPMENT_ROADMAP.md) | Phase-by-phase development status |
| [Building](docs/BUILDING.md) | Build prerequisites, script usage, logs, troubleshooting |
| [Architecture Reference](docs/ARCHITECTURE.md) | Detailed module-by-module reference |
| [Naming Conventions](docs/ATLAS_NAMING_CONVENTIONS.md) | Code style and naming rules |
| [Simulation Philosophy](docs/ATLAS_SIMULATION_PHILOSOPHY.md) | Determinism and simulation design |

## License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.
