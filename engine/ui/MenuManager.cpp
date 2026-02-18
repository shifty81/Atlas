#include "MenuManager.h"

namespace atlas::ui {

// Maximum widget ID to check when iterating. This is a reasonable upper bound
// for editor UI. Could be replaced with UIScreen::WidgetCount() + 1 for
// dynamic sizing, but that would require exposing m_nextId from UIScreen.
static constexpr uint32_t kMaxWidgetId = 1000;

void MenuManager::Init(UIScreen* screen) {
    m_screen = screen;
    m_openMenuId = 0;
}

bool MenuManager::IsPointInWidget(const UIWidget* widget, int32_t x, int32_t y) const {
    if (!widget) return false;
    return x >= widget->x && x < widget->x + widget->width &&
           y >= widget->y && y < widget->y + widget->height;
}

bool MenuManager::HandleEvent(const UIEvent& event) {
    if (!m_screen) return false;

    if (event.type == UIEvent::Type::MouseMove) {
        // Update hover states for all menus and menu items
        for (uint32_t i = 1; i < kMaxWidgetId; ++i) {  // Start at 1 (0 is root)
            UIWidget* widget = m_screen->GetWidgetMutable(i);
            if (!widget) continue;

            if (widget->type == UIWidgetType::Menu) {
                bool wasHovered = widget->isHovered;
                widget->isHovered = IsPointInWidget(widget, event.x, event.y);
                
                // If we hover over a different menu while one is open, switch to it
                if (widget->isHovered && !wasHovered && m_openMenuId != 0 && m_openMenuId != i) {
                    if (UIWidget* oldMenu = m_screen->GetWidgetMutable(m_openMenuId)) {
                        oldMenu->isMenuOpen = false;
                    }
                    m_openMenuId = i;
                    widget->isMenuOpen = true;
                }
            } else if (widget->type == UIWidgetType::MenuItem) {
                // Only update hover for items in the currently open menu
                if (widget->parentId == m_openMenuId && m_openMenuId != 0) {
                    widget->isHovered = IsPointInWidget(widget, event.x, event.y);
                } else {
                    widget->isHovered = false;
                }
            }
        }
        return false; // Don't consume mouse move events
    }

    if (event.type == UIEvent::Type::MouseDown && event.mouseButton == 0) {
        // Check if clicking on a menu button
        bool clickedMenu = false;
        for (uint32_t i = 1; i < kMaxWidgetId; ++i) {
            UIWidget* widget = m_screen->GetWidgetMutable(i);
            if (!widget || widget->type != UIWidgetType::Menu) continue;

            if (IsPointInWidget(widget, event.x, event.y)) {
                clickedMenu = true;
                if (m_openMenuId == i) {
                    // Clicking same menu: close it
                    widget->isMenuOpen = false;
                    m_openMenuId = 0;
                } else {
                    // Clicking different menu: close old, open new
                    if (m_openMenuId != 0) {
                        if (UIWidget* oldMenu = m_screen->GetWidgetMutable(m_openMenuId)) {
                            oldMenu->isMenuOpen = false;
                        }
                    }
                    widget->isMenuOpen = true;
                    m_openMenuId = i;
                }
                return true; // Consumed
            }
        }

        // Check if clicking on a menu item in the open menu
        if (m_openMenuId != 0) {
            for (uint32_t i = 1; i < kMaxWidgetId; ++i) {
                const UIWidget* widget = m_screen->GetWidget(i);
                if (!widget || widget->type != UIWidgetType::MenuItem) continue;
                if (widget->parentId != m_openMenuId) continue;
                if (widget->isSeparator) continue;

                if (IsPointInWidget(widget, event.x, event.y)) {
                    // Invoke callback
                    if (m_menuItemCallback) {
                        m_menuItemCallback(m_openMenuId, i);
                    }
                    
                    // Close the menu
                    if (UIWidget* menu = m_screen->GetWidgetMutable(m_openMenuId)) {
                        menu->isMenuOpen = false;
                    }
                    m_openMenuId = 0;
                    return true; // Consumed
                }
            }
        }

        // Click outside any menu: close open menu
        if (!clickedMenu && m_openMenuId != 0) {
            // Check if click is inside the dropdown area
            bool inDropdown = false;
            for (uint32_t i = 1; i < kMaxWidgetId; ++i) {
                const UIWidget* widget = m_screen->GetWidget(i);
                if (!widget || widget->type != UIWidgetType::MenuItem) continue;
                if (widget->parentId != m_openMenuId) continue;

                if (IsPointInWidget(widget, event.x, event.y)) {
                    inDropdown = true;
                    break;
                }
            }

            if (!inDropdown) {
                if (UIWidget* menu = m_screen->GetWidgetMutable(m_openMenuId)) {
                    menu->isMenuOpen = false;
                }
                m_openMenuId = 0;
                return false; // Don't consume - let click through
            }
        }
    }

    return false;
}

void MenuManager::Update(int32_t mouseX, int32_t mouseY) {
    if (!m_screen) return;

    // Update hover states based on mouse position
    UIEvent moveEvent;
    moveEvent.type = UIEvent::Type::MouseMove;
    moveEvent.x = mouseX;
    moveEvent.y = mouseY;
    HandleEvent(moveEvent);
}

void MenuManager::CloseAllMenus() {
    if (!m_screen || m_openMenuId == 0) return;

    if (UIWidget* menu = m_screen->GetWidgetMutable(m_openMenuId)) {
        menu->isMenuOpen = false;
    }
    m_openMenuId = 0;
}

void MenuManager::SetMenuItemCallback(MenuItemCallback callback) {
    m_menuItemCallback = std::move(callback);
}

} // namespace atlas::ui
