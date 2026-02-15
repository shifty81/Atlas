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
    module/ModuleLoader.cpp
)

target_include_directories(AtlasEngine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/atlas>
)

# Link dynamic loading library on Unix
if(UNIX AND NOT APPLE)
    target_link_libraries(AtlasEngine PUBLIC dl)
elseif(APPLE)
    target_link_libraries(AtlasEngine PUBLIC dl)
endif()

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
