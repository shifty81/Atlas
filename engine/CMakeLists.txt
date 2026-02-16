add_library(AtlasEngine STATIC
    core/Engine.cpp
    core/Logger.cpp
    core/CrashHandler.cpp
    ecs/ECS.cpp
    graphvm/GraphVM.cpp
    graphvm/GraphCompiler.cpp
    graphvm/GraphSerializer.cpp
    assets/AssetRegistry.cpp
    assets/AssetBinary.cpp
    net/NetContext.cpp
    net/Replication.cpp
    sim/TickScheduler.cpp
    world/CubeSphereLayout.cpp
    world/VoxelGridLayout.cpp
    world/TerrainMeshGenerator.cpp
    world/NoiseGenerator.cpp
    world/WorldStreamer.cpp
    world/GalaxyGenerator.cpp
    world/WorldGraph.cpp
    world/WorldNodes.cpp
    world/HeightfieldMesher.cpp
    project/ProjectManager.cpp
    command/CommandHistory.cpp
    interaction/Interaction.cpp
    voice/VoiceCommand.cpp
    plugin/PluginSystem.cpp
    strategygraph/StrategyGraph.cpp
    rules/ServerRules.cpp
    conversation/ConversationGraph.cpp
    ai/AISignalRegistry.cpp
    ai/AIMemory.cpp
    ai/RelationshipModel.cpp
    tile/TileGraph.cpp
    tile/TileNodes.cpp
    sound/SoundGraph.cpp
    sound/SoundNodes.cpp
    ai/BehaviorGraph.cpp
    ai/BehaviorNodes.cpp
    character/CharacterGraph.cpp
    character/CharacterNodes.cpp
    animation/AnimationGraph.cpp
    animation/AnimationNodes.cpp
    weapon/WeaponGraph.cpp
    weapon/WeaponNodes.cpp
    ui/UIGraph.cpp
    ui/UINodes.cpp
    ui/UIScreenGraph.cpp
    ui/UICommandBus.cpp
    ui/UIManager.cpp
    ui/UIRenderer.cpp
    ui/UILayoutSolver.cpp
    flow/GameFlowGraph.cpp
    flow/GameFlowNodes.cpp
    schema/SchemaValidator.cpp
    graphvm/GraphCache.cpp
    ai/FactionRouter.cpp
    asset_graph/AssetGraphExecutor.cpp
    audio/AudioEngine.cpp
    camera/Camera.cpp
    gameplay/MechanicAsset.cpp
    gameplay/SkillTree.cpp
    input/InputManager.cpp
    interaction/InteractionSystem.cpp
    interaction/RuleIntentResolver.cpp
    mod/ModAssetRegistry.cpp
    mod/ModLoader.cpp
    physics/PhysicsWorld.cpp
    story/StoryGraph.cpp
    production/AssetCooker.cpp
    production/BuildProfile.cpp
    production/PlatformTarget.cpp
    sim/ReplayRecorder.cpp
    sim/StateHasher.cpp
    sim/ReplayDivergenceInspector.cpp
    sim/TimeModel.cpp
    sim/WorldState.cpp
    sim/SaveSystem.cpp
    core/DeterministicAllocator.cpp
    core/PermissionManager.cpp
    ui/HUDOverlay.cpp
    module/ModuleLoader.cpp
    render/GLRenderer.cpp
    render/VulkanRenderer.cpp
    platform/X11Window.cpp
)

target_include_directories(AtlasEngine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/atlas>
)

# Floating-point consistency flags — prevent compiler optimizations that
# break determinism across platforms or compilation units.
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(AtlasEngine PRIVATE
        -ffp-contract=off        # Disable FMA contraction
    )
elseif(MSVC)
    target_compile_options(AtlasEngine PRIVATE
        /fp:strict               # Strict floating-point model
    )
endif()

# Link dynamic loading library on Unix
if(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
    find_package(OpenGL REQUIRED)
    find_library(VULKAN_LIB vulkan)
    target_link_libraries(AtlasEngine PUBLIC dl ${X11_LIBRARIES} OpenGL::GL)
    target_include_directories(AtlasEngine PRIVATE ${X11_INCLUDE_DIR})
    if(VULKAN_LIB)
        target_link_libraries(AtlasEngine PUBLIC ${VULKAN_LIB})
    endif()
elseif(APPLE)
    target_link_libraries(AtlasEngine PUBLIC dl)
endif()

# ============================================================
# Simulation / Render Include Firewall
# ============================================================
# AtlasSimulation is an INTERFACE library that exposes ONLY the
# simulation-safe source directories.  Downstream targets that
# link against AtlasSimulation are guaranteed not to pull in
# render, platform, or UI headers, enforcing the sim/render
# separation documented in docs/ATLAS_SIMULATION_PHILOSOPHY.md.
#
# Simulation-safe directories:
#   core, ecs, sim, net, graphvm, assets, world, ai, rules,
#   schema, flow, gameplay, mod, physics, story, command,
#   interaction, plugin, strategygraph, conversation, tile,
#   sound, character, animation, weapon, module, audio, input,
#   production, asset_graph
#
# Render / presentation directories (NOT included):
#   render, platform, ui, camera
# ============================================================
add_library(AtlasSimulation INTERFACE)
target_include_directories(AtlasSimulation INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/core>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ecs>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sim>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/net>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/graphvm>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/assets>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/world>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ai>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/rules>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/schema>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/flow>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/gameplay>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/mod>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/physics>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/story>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/command>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/interaction>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/plugin>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/strategygraph>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/conversation>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/tile>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sound>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/character>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/animation>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/weapon>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/module>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/audio>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/input>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/production>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/asset_graph>
)

# --- Install / Export ---
install(TARGETS AtlasEngine
    EXPORT AtlasEngineTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

# Install public headers preserving directory structure
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include/atlas
    FILES_MATCHING PATTERN "*.h"
)

install(EXPORT AtlasEngineTargets
    FILE AtlasEngineTargets.cmake
    NAMESPACE Atlas::
    DESTINATION lib/cmake/AtlasEngine
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/AtlasEngineConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/AtlasEngineConfig.cmake"
    INSTALL_DESTINATION lib/cmake/AtlasEngine
)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/AtlasEngineConfigVersion.cmake"
    VERSION 1.0.0
    COMPATIBILITY SameMajorVersion
)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/AtlasEngineConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/AtlasEngineConfigVersion.cmake"
    DESTINATION lib/cmake/AtlasEngine
)
