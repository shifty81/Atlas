#pragma once
#include "UIGraph.h"
#include "UIScreenGraph.h"
#include "UICommandBus.h"
#include "UIRenderer.h"
#include "UIEventRouter.h"
#include "FontBootstrap.h"
#include <string>

namespace atlas::ui {

enum class GUIContext : uint8_t {
    Editor,
    Game,
    Server
};

class UIManager {
public:
    void Init(GUIContext context);
    void Shutdown();

    void SetContext(GUIContext context);
    GUIContext GetContext() const;

    void Update(const UIContext& ctx);

    void SetRenderer(UIRenderer* renderer);
    UIRenderer* GetRenderer() const;

    void Render(UIRenderer* renderer);

    UIScreen& GetScreen();
    const UIScreen& GetScreen() const;

    UIGraph& GetGraph();
    const UIGraph& GetGraph() const;

    UICommandBus& GetCommandBus();

    bool IsInitialized() const;

    // --- Viewport & DPI ---

    /// Notify the UI system that the window has been resized.
    void SetViewportSize(float width, float height);

    /// Returns the current viewport width.
    float GetViewportWidth() const;

    /// Returns the current viewport height.
    float GetViewportHeight() const;

    /// Set the display DPI scale factor.
    void SetDPIScale(float scale);

    /// Returns the current DPI scale factor.
    float GetDPIScale() const;

    // --- Input Routing ---

    /// Access the centralized event router for UI input dispatch.
    UIEventRouter& GetEventRouter();
    const UIEventRouter& GetEventRouter() const;

    /// Dispatch a platform input event through the UI layer.
    /// Returns true if the event was consumed by a UI element.
    bool DispatchEvent(const UIEvent& event);

    // --- Font System ---

    /// Access the font bootstrap for font lifecycle management.
    FontBootstrap& GetFontBootstrap();
    const FontBootstrap& GetFontBootstrap() const;

    /// Returns true when the font system is ready for text rendering.
    bool IsFontReady() const;

private:
    void RenderWidget(UIRenderer* renderer, uint32_t widgetId, int depth = 0);
    static constexpr int kMaxRenderDepth = 64;

    GUIContext m_context = GUIContext::Editor;
    UIScreen m_screen;
    UIGraph m_graph;
    UICommandBus m_commandBus;
    UIEventRouter m_eventRouter;
    FontBootstrap m_fontBootstrap;
    UIRenderer* m_renderer = nullptr;
    float m_viewportWidth = 0.0f;
    float m_viewportHeight = 0.0f;
    float m_dpiScale = 1.0f;
    bool m_initialized = false;
};

} // namespace atlas::ui
