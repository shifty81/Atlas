add_executable(AtlasEditor
    main.cpp
    ui/EditorLayout.cpp
    panels/ECSInspectorPanel.cpp
    panels/NetInspectorPanel.cpp
    panels/ConsolePanel.cpp
    panels/InteractionDebugger.cpp
    tools/GamePackagerPanel.cpp
    assistant/EditorAssistant.cpp
    assistant/AssetGraphAssistant.cpp
)

target_link_libraries(AtlasEditor AtlasEngine)
