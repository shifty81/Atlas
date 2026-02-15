add_executable(AtlasEditor
    main.cpp
    ui/EditorLayout.cpp
    panels/ECSInspectorPanel.cpp
    panels/NetInspectorPanel.cpp
    panels/ConsolePanel.cpp
    panels/InteractionDebugger.cpp
    panels/ProjectPickerPanel.cpp
    panels/WorldGraphPanel.cpp
    panels/AssetBrowserPanel.cpp
    panels/VoiceCommandPanel.cpp
    panels/InteractionDebugPanel.cpp
    tools/GamePackagerPanel.cpp
    assistant/EditorAssistant.cpp
    assistant/AssetGraphAssistant.cpp
)

target_link_libraries(AtlasEditor AtlasEngine)
