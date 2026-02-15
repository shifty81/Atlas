# Atlas Repo Split & Game Module Plan

Goal: keep the Atlas repo purely engine/runtime, ship generic client/server/editor binaries, and host game-specific logic (EveOffline or others) as external modules.

## Target Layout

```
Atlas/                        (engine repo)
├─ engine/                    ← AtlasEngine static lib (core, ecs, net, sim, assets, graphs)
├─ runtime/                   ← AtlasRuntime entry that loads a game module
├─ client/                    ← AtlasClient links AtlasEngine + AtlasGameplay + module
├─ server/                    ← AtlasServer links AtlasEngine + AtlasGameplay + module
├─ editor/                    ← AtlasEditor links AtlasEngine + AtlasGameplay + module
├─ modules/
│  └─ atlas_gameplay/         ← AtlasGameplay static lib (factions, economy, combat base, AI schedulers)
└─ CMakeLists.txt             ← adds AtlasEngine, AtlasGameplay, AtlasRuntime/Client/Server/Editor

Atlas-EveOffline/             (game repo)
├─ game/
│  └─ EveOfflineModule.cpp    ← builds EveOfflineModule shared lib
├─ assets/
└─ CMakeLists.txt             ← finds AtlasEngine + AtlasGameplay and exports CreateGameModule()
```

Recommended CMake targets:
- `AtlasEngine` (static) — existing core
- `AtlasGameplay` (static) — reusable gameplay frameworks, no Eve names
- `AtlasRuntime` — generic bootstrap that loads a module
- `AtlasServer` / `AtlasClient` / `AtlasEditor` — link AtlasEngine + AtlasGameplay; call into module
- `EveOfflineModule` (shared, in game repo) — implements the module interface

Example CMake wiring (engine repo):
```cmake
add_library(AtlasGameplay STATIC
    modules/atlas_gameplay/FactionSystem.cpp
    modules/atlas_gameplay/CombatFramework.cpp
    modules/atlas_gameplay/EconomySystem.cpp)
target_link_libraries(AtlasGameplay PUBLIC AtlasEngine)

add_executable(AtlasServer server/main.cpp)
target_link_libraries(AtlasServer PRIVATE AtlasEngine AtlasGameplay)
```

Game repo (EveOffline) links against installed/packaged engine artifacts:
```cmake
find_package(AtlasEngine REQUIRED)
add_library(EveOfflineModule SHARED game/EveOfflineModule.cpp)
target_link_libraries(EveOfflineModule PRIVATE AtlasEngine AtlasGameplay)
target_compile_definitions(EveOfflineModule PRIVATE ATLAS_GAME_MODULE_NAME=\"EveOffline\")
```

## Game Module Interface

Define a narrow interface so the server/client stay generic. Suggested header (lives in AtlasEngine include path):

```cpp
namespace atlas::module {
struct GameModuleDesc {
    const char* name;
    uint32_t version;
};

struct GameModuleContext {
    ecs::World& world;
    net::NetContext& net;
    net::ReplicationManager& replication;
    rules::ServerRules& rules;
    assets::AssetRegistry& assets;
    project::ProjectDescriptor project;
};

class IGameModule {
public:
    virtual GameModuleDesc Describe() const = 0;
    virtual void RegisterTypes(GameModuleContext& ctx) = 0;          // components, systems, assets
    virtual void ConfigureReplication(GameModuleContext& ctx) {}     // add ReplicationRule entries
    virtual void ConfigureServerRules(GameModuleContext& ctx) {}     // populate ServerRules
    virtual void OnStart(GameModuleContext& ctx) {}                  // called after project load
    virtual void OnTick(GameModuleContext& ctx, float dt) {}         // per-tick hook
    virtual void OnShutdown(GameModuleContext& ctx) {}               // cleanup
    virtual ~IGameModule() = default;
};

using CreateGameModuleFn = std::unique_ptr<IGameModule>(*)();
} // namespace atlas::module

// Factory symbol exported by each game module (dll/so):
extern "C" std::unique_ptr<atlas::module::IGameModule> CreateGameModule();
```

## Runtime Loading Flow

`ServerRuntime` / `ClientRuntime` (in Atlas repo) should:
1. Resolve module name from CLI or project file (e.g., `project.modules.content`).
2. Locate the shared library (`libEveOfflineModule.so`, `EveOfflineModule.dll`) or a statically linked fallback.
3. Load with `dlopen` / `GetProcAddress` (or a compile-time registry in tests).
4. Call `CreateGameModule()` to get the module instance.
5. Build `GameModuleContext` with `Engine` subsystems (world, net, replication, rules, assets, project descriptor).
6. Invoke `RegisterTypes`, `ConfigureReplication`, `ConfigureServerRules`, then `OnStart`.
7. On each tick, forward to `OnTick` with `dt`.
8. On shutdown, call `OnShutdown` and unload the library.

Failure policy: if the module cannot be loaded or `Describe().name` mismatches the requested project, log + exit with a clear message.

## Migration Steps (ordered)

1) **Freeze engine-facing APIs**: finalize `ecs::World`, `net::NetContext`, `net::ReplicationManager`, `rules::ServerRules`, and `assets::AssetRegistry` headers as the public surface.
2) **Carve AtlasGameplay**: move reusable gameplay frameworks (faction router, diplomacy, economy scaffolding, combat base, AI schedulers) under `modules/atlas_gameplay` and expose them via `AtlasGameplay` target; prohibit Eve-specific names.
3) **Add module loader**: implement the interface header, add loader code to `runtime/` (shared helper used by server/client/editor), and add a CMake option `ATLAS_STATIC_GAME_MODULE` for statically linking a module during tests.
4) **Split EveOffline repo**: create `Atlas-EveOffline` as a consumer that builds `EveOfflineModule` and embeds assets; reference Atlas as a submodule or packaged SDK. The server/client binaries remain the same; only the module changes.
5) **CI/build updates**: extend `build.sh` targets for `engine`, `client`, `server`, and `module` (when present), and publish engine SDK artifacts so external modules can `find_package(AtlasEngine)` cleanly.
