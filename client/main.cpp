#include "core/Engine.h"
#include "ui/DiagnosticsOverlay.h"
#include <iostream>

int main() {
    atlas::EngineConfig cfg;
    cfg.mode = atlas::EngineMode::Client;

    atlas::Engine engine(cfg);
    engine.InitCore();
    engine.InitRender();
    engine.InitUI();
    engine.InitECS();
    engine.InitNetworking();

    // Diagnostics overlay is available (off by default in client;
    // toggle at runtime with Ctrl+`).

    engine.Run();

    return 0;
}
