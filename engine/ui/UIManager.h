#pragma once
#include "UIGraph.h"
#include "UIScreenGraph.h"
#include "UICommandBus.h"
#include "UIRenderer.h"
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

private:
    void RenderWidget(UIRenderer* renderer, uint32_t widgetId, int depth = 0);
    static constexpr int kMaxRenderDepth = 64;

    GUIContext m_context = GUIContext::Editor;
    UIScreen m_screen;
    UIGraph m_graph;
    UICommandBus m_commandBus;
    UIRenderer* m_renderer = nullptr;
    bool m_initialized = false;
};

} // namespace atlas::ui
