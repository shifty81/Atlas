#pragma once
#include "UIGraph.h"
#include "UIScreenGraph.h"
#include "UICommandBus.h"
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

    UIScreen& GetScreen();
    const UIScreen& GetScreen() const;

    UIGraph& GetGraph();
    const UIGraph& GetGraph() const;

    UICommandBus& GetCommandBus();

    bool IsInitialized() const;

private:
    GUIContext m_context = GUIContext::Editor;
    UIScreen m_screen;
    UIGraph m_graph;
    UICommandBus m_commandBus;
    bool m_initialized = false;
};

} // namespace atlas::ui
