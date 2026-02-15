#include "UIManager.h"

namespace atlas::ui {

void UIManager::Init(GUIContext context) {
    m_context = context;

    std::string screenName;
    switch (context) {
        case GUIContext::Editor: screenName = "EditorScreen"; break;
        case GUIContext::Game:   screenName = "GameScreen";   break;
        case GUIContext::Server: screenName = "ServerScreen"; break;
    }

    m_screen.Init(screenName);
    m_initialized = true;
}

void UIManager::Shutdown() {
    m_commandBus.Clear();
    m_initialized = false;
}

void UIManager::SetContext(GUIContext context) {
    m_context = context;
}

GUIContext UIManager::GetContext() const {
    return m_context;
}

void UIManager::Update(const UIContext& ctx) {
    if (!m_initialized) return;

    m_commandBus.Dispatch();

    if (m_graph.IsCompiled()) {
        m_graph.Execute(ctx);
    }
}

UIScreen& UIManager::GetScreen() {
    return m_screen;
}

const UIScreen& UIManager::GetScreen() const {
    return m_screen;
}

UIGraph& UIManager::GetGraph() {
    return m_graph;
}

const UIGraph& UIManager::GetGraph() const {
    return m_graph;
}

UICommandBus& UIManager::GetCommandBus() {
    return m_commandBus;
}

bool UIManager::IsInitialized() const {
    return m_initialized;
}

} // namespace atlas::ui
