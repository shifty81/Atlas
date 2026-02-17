#include "core/Engine.h"
#include "core/EnginePhase.h"
#include "core/Logger.h"
#include "ui/UIScreenGraph.h"
#include "ui/GUIDSLParser.h"
#include "ui/DefaultEditorLayout.h"
#include "ui/EditorAttachProtocol.h"
#include "ui/EditorTheme.h"
#include "ui/FontBootstrap.h"
#include "ui/DiagnosticsOverlay.h"
#include "ui/LauncherScreen.h"
#include <iostream>

/// Names of panels that are non-closable (always present in the dock tree).
static const char* kMainPanels[] = {
    "AssetBrowser",
    "Viewport",
    "Inspector",
    "ReplayTimeline",
    "Console",
    "Permissions",
    "DeterminismStatus",
};

/// Recursively build widgets from a parsed DSL node tree.
static void ApplyDSLNode(atlas::ui::UIScreen& screen,
                         const atlas::ui::DSLNode& node,
                         uint32_t parentWidget,
                         int x, int y, int w, int h) {
    switch (node.type) {
    case atlas::ui::DSLNodeType::Panel: {
        screen.AddWidget(atlas::ui::UIWidgetType::Panel, node.name, x, y, w, h);
        break;
    }
    case atlas::ui::DSLNodeType::Split: {
        if (node.children.size() < 2) break;
        float ratio = node.ratio;
        if (node.splitDir == atlas::ui::DSLSplitDir::Horizontal) {
            int leftW = static_cast<int>(w * ratio);
            int rightW = w - leftW;
            ApplyDSLNode(screen, *node.children[0], parentWidget, x, y, leftW, h);
            ApplyDSLNode(screen, *node.children[1], parentWidget, x + leftW, y, rightW, h);
        } else {
            int topH = static_cast<int>(h * ratio);
            int botH = h - topH;
            ApplyDSLNode(screen, *node.children[0], parentWidget, x, y, w, topH);
            ApplyDSLNode(screen, *node.children[1], parentWidget, x, y + topH, w, botH);
        }
        break;
    }
    case atlas::ui::DSLNodeType::Dock: {
        float ratio = node.ratio;
        int dockX = x, dockY = y, dockW = w, dockH = h;
        switch (node.dockTarget) {
        case atlas::ui::DSLDockTarget::Right:
            dockW = static_cast<int>(w * ratio);
            dockX = x + w - dockW;
            break;
        case atlas::ui::DSLDockTarget::Left:
            dockW = static_cast<int>(w * ratio);
            break;
        case atlas::ui::DSLDockTarget::Bottom:
            dockH = static_cast<int>(h * ratio);
            dockY = y + h - dockH;
            break;
        case atlas::ui::DSLDockTarget::Top:
            dockH = static_cast<int>(h * ratio);
            break;
        default: break;
        }
        for (auto& child : node.children)
            ApplyDSLNode(screen, *child, parentWidget, dockX, dockY, dockW, dockH);
        break;
    }
    case atlas::ui::DSLNodeType::Tabs: {
        for (auto& child : node.children)
            ApplyDSLNode(screen, *child, parentWidget, x, y, w, h);
        break;
    }
    case atlas::ui::DSLNodeType::Layout: {
        for (auto& child : node.children)
            ApplyDSLNode(screen, *child, parentWidget, x, y, w, h);
        break;
    }
    }
}

static void BuildEditorUI(atlas::ui::UIScreen& screen) {
    // Self-hosting: parse the default editor DSL to build the initial
    // widget tree.  If DSL parsing fails, fall back to a hard-coded layout.
    atlas::ui::GUIDSLParser parser;
    auto result = parser.Parse(atlas::editor::DefaultEditorDSL());

    // Apply editor theme colors to the hard-coded fallback widgets.
    atlas::editor::EditorTheme theme = atlas::editor::EditorTheme::Dark();
    (void)theme; // Theme values are used by the renderer; stored for future use.

    if (result.success && result.root) {
        // parentWidget=0 is the root; x,y=0,0 is the top-left; 1280x720 is default viewport
        ApplyDSLNode(screen, *result.root, 0, 0, 0, 1280, 720);
        return;
    }

    if (!result.success) {
        std::cerr << "DSL parse error: " << result.errorMessage << std::endl;
    }

    // Fallback: hard-coded layout
    // Menu bar
    uint32_t menuBar = screen.AddWidget(atlas::ui::UIWidgetType::Panel, "MenuBar",
                                         0, 0, 1280, 28);

    uint32_t fileBtn = screen.AddWidget(atlas::ui::UIWidgetType::Button, "File",
                                         4, 2, 50, 24);
    screen.SetParent(fileBtn, menuBar);

    uint32_t editBtn = screen.AddWidget(atlas::ui::UIWidgetType::Button, "Edit",
                                         58, 2, 50, 24);
    screen.SetParent(editBtn, menuBar);

    uint32_t viewBtn = screen.AddWidget(atlas::ui::UIWidgetType::Button, "View",
                                         112, 2, 50, 24);
    screen.SetParent(viewBtn, menuBar);

    uint32_t toolsBtn = screen.AddWidget(atlas::ui::UIWidgetType::Button, "Tools",
                                          166, 2, 55, 24);
    screen.SetParent(toolsBtn, menuBar);

    // Left panel — Project / Asset Browser
    uint32_t leftPanel = screen.AddWidget(atlas::ui::UIWidgetType::Panel, "AssetBrowser",
                                           0, 30, 260, 690);

    uint32_t assetTitle = screen.AddWidget(atlas::ui::UIWidgetType::Text, "Asset Browser",
                                            4, 34, 252, 20);
    screen.SetParent(assetTitle, leftPanel);

    uint32_t assetList = screen.AddWidget(atlas::ui::UIWidgetType::List, "Assets",
                                           4, 56, 252, 600);
    screen.SetParent(assetList, leftPanel);

    // Center panel — Viewport / World Graph
    uint32_t centerPanel = screen.AddWidget(atlas::ui::UIWidgetType::Panel, "Viewport",
                                             262, 30, 756, 480);

    uint32_t vpTitle = screen.AddWidget(atlas::ui::UIWidgetType::Text, "Viewport",
                                         266, 34, 748, 20);
    screen.SetParent(vpTitle, centerPanel);

    // Right panel — Inspector
    uint32_t rightPanel = screen.AddWidget(atlas::ui::UIWidgetType::Panel, "Inspector",
                                            1020, 30, 260, 690);

    uint32_t inspTitle = screen.AddWidget(atlas::ui::UIWidgetType::Text, "ECS Inspector",
                                           1024, 34, 252, 20);
    screen.SetParent(inspTitle, rightPanel);

    uint32_t entityList = screen.AddWidget(atlas::ui::UIWidgetType::List, "Entities",
                                            1024, 56, 252, 600);
    screen.SetParent(entityList, rightPanel);

    // Bottom panel — Console
    uint32_t bottomPanel = screen.AddWidget(atlas::ui::UIWidgetType::Panel, "Console",
                                             262, 512, 756, 208);

    uint32_t consoleTitle = screen.AddWidget(atlas::ui::UIWidgetType::Text, "Console",
                                              266, 516, 748, 20);
    screen.SetParent(consoleTitle, bottomPanel);

    uint32_t consoleInput = screen.AddWidget(atlas::ui::UIWidgetType::InputField, "command...",
                                              266, 692, 748, 24);
    screen.SetParent(consoleInput, bottomPanel);
}

int main() {
    atlas::EngineConfig cfg;
    cfg.mode = atlas::EngineMode::Editor;

    atlas::Engine engine(cfg);
    engine.InitCore();
    engine.InitRender();
    engine.InitUI();
    engine.InitECS();
    engine.InitNetworking();
    engine.InitEditor();

    // --- Font bootstrap (must run after renderer init) ---
    atlas::ui::FontBootstrap fontBootstrap;
    fontBootstrap.Init(cfg.assetRoot);

    // Set up default attach mode (standalone)
    atlas::editor::EditorAttachProtocol attach;
    attach.Init();
    atlas::editor::AttachConfig attachCfg;
    attachCfg.mode = atlas::editor::AttachMode::Standalone;
    attach.Connect(attachCfg);

    // --- Launcher screen: scan for projects ---
    atlas::editor::LauncherScreen launcher;
    launcher.ScanProjects("projects");

    if (launcher.IsQuitRequested()) {
        fontBootstrap.Shutdown();
        return 0;
    }

    // Auto-select first project if one was found
    if (!launcher.Projects().empty() && !launcher.IsProjectChosen()) {
        launcher.SelectProject(0);
        launcher.ConfirmSelection();
        if (auto* proj = launcher.SelectedProject()) {
            atlas::Logger::Info("Launcher: opening project '" + proj->name
                                + "' (" + proj->path + ")");
        }
    }

    BuildEditorUI(engine.GetUIManager().GetScreen());

    // Enable diagnostics overlay by default in editor
    atlas::ui::DiagnosticsOverlay::SetEnabled(true);

    engine.Run();

    fontBootstrap.Shutdown();

    return 0;
}
