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
    m_fontBootstrap.Shutdown();
    m_eventRouter.Clear();
    m_commandBus.Clear();
    m_renderer = nullptr;
    m_viewportWidth = 0.0f;
    m_viewportHeight = 0.0f;
    m_dpiScale = 1.0f;
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

void UIManager::SetRenderer(UIRenderer* renderer) {
    m_renderer = renderer;
}

UIRenderer* UIManager::GetRenderer() const {
    return m_renderer;
}

void UIManager::Render(UIRenderer* renderer) {
    if (!m_initialized || !renderer) return;

    // Walk root-level widgets (parentId == 0) and render them
    auto roots = m_screen.GetChildren(0);
    for (uint32_t id : roots) {
        RenderWidget(renderer, id);
    }
}

void UIManager::RenderWidget(UIRenderer* renderer, uint32_t widgetId, int depth) {
    if (depth >= kMaxRenderDepth) return;
    const UIWidget* widget = m_screen.GetWidget(widgetId);
    if (!widget || !widget->visible) return;

    UIRect rect;
    rect.x = static_cast<int32_t>(widget->x);
    rect.y = static_cast<int32_t>(widget->y);
    rect.w = static_cast<int32_t>(widget->width);
    rect.h = static_cast<int32_t>(widget->height);

    switch (widget->type) {
        case UIWidgetType::Panel: {
            UIColor bg = {43, 43, 43, 255};
            renderer->DrawRect(rect, bg);
            UIColor border = {70, 73, 75, 255};
            renderer->DrawBorder(rect, 1, border);
            break;
        }
        case UIWidgetType::Button: {
            UIColor bg = {55, 58, 62, 255};
            renderer->DrawRect(rect, bg);
            UIColor border = {80, 83, 88, 255};
            renderer->DrawBorder(rect, 1, border);
            UIColor textColor = {220, 220, 220, 255};
            renderer->DrawText(rect, widget->name, textColor);
            break;
        }
        case UIWidgetType::Text: {
            UIColor textColor = {220, 220, 220, 255};
            renderer->DrawText(rect, widget->name, textColor);
            break;
        }
        case UIWidgetType::Image: {
            UIColor tint = {255, 255, 255, 255};
            renderer->DrawImage(rect, 0, tint);
            break;
        }
        case UIWidgetType::List: {
            UIColor bg = {35, 37, 40, 255};
            renderer->DrawRect(rect, bg);
            UIColor border = {70, 73, 75, 255};
            renderer->DrawBorder(rect, 1, border);
            break;
        }
        case UIWidgetType::SlotGrid: {
            UIColor bg = {43, 43, 43, 255};
            renderer->DrawRect(rect, bg);
            UIColor border = {70, 73, 75, 255};
            renderer->DrawBorder(rect, 1, border);
            break;
        }
        case UIWidgetType::InputField: {
            UIColor bg = {35, 37, 40, 255};
            renderer->DrawRect(rect, bg);
            UIColor border = {70, 100, 150, 255};
            renderer->DrawBorder(rect, 1, border);
            UIColor textColor = {160, 160, 160, 255};
            renderer->DrawText(rect, widget->name, textColor);
            break;
        }
    }

    // Render children
    auto children = m_screen.GetChildren(widgetId);
    for (uint32_t childId : children) {
        RenderWidget(renderer, childId, depth + 1);
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

void UIManager::SetViewportSize(float width, float height) {
    m_viewportWidth = width;
    m_viewportHeight = height;
}

float UIManager::GetViewportWidth() const {
    return m_viewportWidth;
}

float UIManager::GetViewportHeight() const {
    return m_viewportHeight;
}

void UIManager::SetDPIScale(float scale) {
    m_dpiScale = scale;
}

float UIManager::GetDPIScale() const {
    return m_dpiScale;
}

UIEventRouter& UIManager::GetEventRouter() {
    return m_eventRouter;
}

const UIEventRouter& UIManager::GetEventRouter() const {
    return m_eventRouter;
}

bool UIManager::DispatchEvent(const UIEvent& event) {
    if (!m_initialized) return false;
    return m_eventRouter.Dispatch(event);
}

FontBootstrap& UIManager::GetFontBootstrap() {
    return m_fontBootstrap;
}

const FontBootstrap& UIManager::GetFontBootstrap() const {
    return m_fontBootstrap;
}

bool UIManager::IsFontReady() const {
    return m_fontBootstrap.IsReady();
}

} // namespace atlas::ui
