add_executable(AtlasTests
    main.cpp
    test_graphvm.cpp
    test_ecs.cpp
    test_components.cpp
    test_asset.cpp
    test_net.cpp
    test_net_queue.cpp
    test_world.cpp
    test_compiler.cpp
    test_engine.cpp
    test_console.cpp
    test_terrain.cpp
    test_noise.cpp
    test_streaming.cpp
    test_galaxy.cpp
    test_interaction.cpp
    test_diplomacy.cpp
    test_asset_graph.cpp
    test_mod.cpp
    test_editor_assistant.cpp
    ${CMAKE_SOURCE_DIR}/editor/panels/ConsolePanel.cpp
    ${CMAKE_SOURCE_DIR}/editor/panels/InteractionDebugger.cpp
    ${CMAKE_SOURCE_DIR}/editor/assistant/EditorAssistant.cpp
)

target_link_libraries(AtlasTests AtlasEngine)
target_include_directories(AtlasTests PRIVATE ${CMAKE_SOURCE_DIR}/editor)

add_test(NAME AtlasTests COMMAND AtlasTests)
