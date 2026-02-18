#include "UIManager.h"

namespace atlas::ui {

// --- MenuItem rendering constants ---
static constexpr const char* kCheckmarkSymbol = "\xe2\x9c\x93"; // ✓

/// Compute horizontal offset for icon placement, accounting for checkmark space.
static int32_t IconOffsetX(int32_t rectX, bool isCheckable) {
    return rectX + (isCheckable ? 18 : 2);
}

void UIManager::Init(GUIContext context) {
    m_context = context;

    std::string screenName;
    switch (context) {
        case GUIContext::Editor: screenName = "EditorScreen"; break;
        case GUIContext::Game:   screenName = "GameScreen";   break;
        case GUIContext::Server: screenName = "ServerScreen"; break;
    }

    m_screen.Init(screenName);
    m_menuManager.Init(&m_screen);
    m_tabManager.Init(&m_screen);
    m_scrollManager.Init(&m_screen);
    m_toolbarManager.Init(&m_screen);
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
        case UIWidgetType::Menu: {
            // Menu button in menu bar
            UIColor bg = widget->isMenuOpen ? UIColor{65, 68, 72, 255} : UIColor{43, 43, 43, 255};
            renderer->DrawRect(rect, bg);
            if (widget->isHovered || widget->isMenuOpen) {
                UIColor highlight = {75, 78, 82, 255};
                renderer->DrawRect(rect, highlight);
            }
            UIColor textColor = {220, 220, 220, 255};
            renderer->DrawText(rect, widget->name, textColor);
            break;
        }
        case UIWidgetType::MenuItem: {
            if (widget->isSeparator) {
                // Draw separator line
                UIColor separatorColor = {70, 73, 75, 255};
                UIRect sepRect = {rect.x + 4, rect.y + rect.h / 2, rect.w - 8, 1};
                renderer->DrawRect(sepRect, separatorColor);
            } else if (widget->isDisabled) {
                // Disabled menu item — grayed-out text, no hover highlight
                UIColor bg = {45, 47, 50, 255};
                renderer->DrawRect(rect, bg);
                UIColor textColor = {100, 100, 100, 255};
                renderer->DrawText(rect, widget->name, textColor);
                // Draw shortcut label if present, also grayed out
                if (!widget->shortcutLabel.empty()) {
                    UIRect shortcutRect = {rect.x + rect.w - 80, rect.y, 70, rect.h};
                    renderer->DrawText(shortcutRect, widget->shortcutLabel, textColor);
                }
                // Draw icon if present, grayed out
                if (widget->iconId != 0) {
                    UIColor iconTint = {100, 100, 100, 255};
                    UIRect iconRect = {IconOffsetX(rect.x, widget->isCheckable), rect.y + 2, rect.h - 4, rect.h - 4};
                    renderer->DrawIcon(iconRect, widget->iconId, iconTint);
                }
            } else {
                // Normal menu item
                UIColor bg = widget->isHovered ? UIColor{65, 115, 180, 255} : UIColor{45, 47, 50, 255};
                renderer->DrawRect(rect, bg);
                // Checkmark indicator
                if (widget->isCheckable) {
                    UIColor checkColor = widget->isChecked ? UIColor{220, 220, 220, 255} : UIColor{80, 80, 80, 255};
                    UIRect checkRect = {rect.x + 2, rect.y, 16, rect.h};
                    renderer->DrawText(checkRect, widget->isChecked ? kCheckmarkSymbol : " ", checkColor);
                }
                // Icon rendering
                if (widget->iconId != 0) {
                    UIColor iconTint = {255, 255, 255, 255};
                    UIRect iconRect = {IconOffsetX(rect.x, widget->isCheckable), rect.y + 2, rect.h - 4, rect.h - 4};
                    renderer->DrawIcon(iconRect, widget->iconId, iconTint);
                }
                UIColor textColor = {220, 220, 220, 255};
                renderer->DrawText(rect, widget->name, textColor);
                // Draw shortcut label right-aligned if present
                if (!widget->shortcutLabel.empty()) {
                    UIColor shortcutColor = {160, 160, 160, 255};
                    UIRect shortcutRect = {rect.x + rect.w - 80, rect.y, 70, rect.h};
                    renderer->DrawText(shortcutRect, widget->shortcutLabel, shortcutColor);
                }
                // Draw submenu indicator arrow if this item has a submenu
                if (widget->hasSubmenu) {
                    UIColor arrowColor = {180, 180, 180, 255};
                    UIRect arrowRect = {rect.x + rect.w - 16, rect.y, 12, rect.h};
                    renderer->DrawText(arrowRect, ">", arrowColor);
                }
            }
            break;
        }
        case UIWidgetType::Toolbar: {
            UIColor bg = {50, 52, 56, 255};
            renderer->DrawRect(rect, bg);
            UIColor borderBottom = {70, 73, 75, 255};
            UIRect bottomLine = {rect.x, rect.y + rect.h - 1, rect.w, 1};
            renderer->DrawRect(bottomLine, borderBottom);
            break;
        }
        case UIWidgetType::StatusBar: {
            UIColor bg = {30, 31, 34, 255};
            renderer->DrawRect(rect, bg);
            UIColor borderTop = {70, 73, 75, 255};
            UIRect topLine = {rect.x, rect.y, rect.w, 1};
            renderer->DrawRect(topLine, borderTop);
            UIColor textColor = {160, 160, 160, 255};
            renderer->DrawText(rect, widget->name, textColor);
            break;
        }
        case UIWidgetType::Tooltip: {
            UIColor bg = {60, 62, 66, 240};
            renderer->DrawRect(rect, bg);
            UIColor border = {100, 103, 108, 255};
            renderer->DrawBorder(rect, 1, border);
            UIColor textColor = {220, 220, 220, 255};
            renderer->DrawText(rect, widget->name, textColor);
            break;
        }
        case UIWidgetType::Tab: {
            UIColor bg = widget->isHovered ? UIColor{55, 58, 62, 255} : UIColor{43, 43, 43, 255};
            renderer->DrawRect(rect, bg);
            if (widget->isChecked) {
                // Active tab: highlight bottom border
                UIColor activeBar = {65, 115, 180, 255};
                UIRect barRect = {rect.x, rect.y + rect.h - 2, rect.w, 2};
                renderer->DrawRect(barRect, activeBar);
            }
            UIColor textColor = widget->isChecked ? UIColor{220, 220, 220, 255} : UIColor{160, 160, 160, 255};
            renderer->DrawText(rect, widget->name, textColor);
            break;
        }
        case UIWidgetType::ScrollView: {
            UIColor bg = {35, 37, 40, 255};
            renderer->DrawRect(rect, bg);
            UIColor border = {70, 73, 75, 255};
            renderer->DrawBorder(rect, 1, border);
            break;
        }
        case UIWidgetType::DockArea: {
            UIColor bg = {38, 40, 43, 255};
            renderer->DrawRect(rect, bg);
            UIColor border = {60, 63, 67, 255};
            renderer->DrawBorder(rect, 1, border);
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
    
    // Let menu manager handle the event first
    if (m_menuManager.HandleEvent(event)) {
        return true;
    }

    // Handle scroll wheel events via ScrollManager
    if (event.type == UIEvent::Type::ScrollWheel) {
        if (m_scrollManager.HandleScrollWheel(event.x, event.y, event.scrollDelta)) {
            return true;
        }
    }

    // Handle tab clicks
    if (event.type == UIEvent::Type::MouseDown && event.mouseButton == 0) {
        if (m_tabManager.HandleClick(event.x, event.y)) {
            return true;
        }
        if (m_toolbarManager.HandleClick(event.x, event.y)) {
            return true;
        }
    }
    
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

MenuManager& UIManager::GetMenuManager() {
    return m_menuManager;
}

const MenuManager& UIManager::GetMenuManager() const {
    return m_menuManager;
}

TabManager& UIManager::GetTabManager() {
    return m_tabManager;
}

const TabManager& UIManager::GetTabManager() const {
    return m_tabManager;
}

ScrollManager& UIManager::GetScrollManager() {
    return m_scrollManager;
}

const ScrollManager& UIManager::GetScrollManager() const {
    return m_scrollManager;
}

ToolbarManager& UIManager::GetToolbarManager() {
    return m_toolbarManager;
}

const ToolbarManager& UIManager::GetToolbarManager() const {
    return m_toolbarManager;
}

} // namespace atlas::ui
