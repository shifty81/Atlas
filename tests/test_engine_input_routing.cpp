#include "../engine/core/Engine.h"
#include "../engine/ui/UIEventRouter.h"
#include "../engine/ui/DiagnosticsOverlay.h"
#include <iostream>
#include <cassert>

using namespace atlas;

// ============================================================
// Engine Input Routing Tests
// ============================================================

void test_engine_has_event_router() {
    EngineConfig cfg;
    cfg.mode = EngineMode::Server;
    cfg.headless = true;

    Engine engine(cfg);
    engine.InitCore();

    // EventRouter should be accessible and empty
    auto& router = engine.GetEventRouter();
    assert(router.TargetCount() == 0);

    std::cout << "[PASS] test_engine_has_event_router" << std::endl;
}

void test_engine_mouse_tracking_defaults() {
    EngineConfig cfg;
    cfg.mode = EngineMode::Server;
    cfg.headless = true;

    Engine engine(cfg);
    engine.InitCore();

    // Mouse position should default to 0,0
    assert(engine.MouseX() == 0);
    assert(engine.MouseY() == 0);

    std::cout << "[PASS] test_engine_mouse_tracking_defaults" << std::endl;
}

void test_engine_event_router_register() {
    EngineConfig cfg;
    cfg.mode = EngineMode::Server;
    cfg.headless = true;

    Engine engine(cfg);
    engine.InitCore();

    // Create a simple event target
    struct TestTarget : public ui::UIEventTarget {
        bool HitTest(int32_t, int32_t) const override { return true; }
        bool OnEvent(const ui::UIEvent&) override {
            ++eventCount;
            return true;
        }
        int eventCount = 0;
    };

    TestTarget target;
    engine.GetEventRouter().Register(&target);
    assert(engine.GetEventRouter().TargetCount() == 1);

    // Dispatch an event through the router
    ui::UIEvent evt;
    evt.type = ui::UIEvent::Type::MouseDown;
    evt.x = 10;
    evt.y = 20;
    engine.GetEventRouter().Dispatch(evt);

    assert(target.eventCount == 1);

    engine.GetEventRouter().Unregister(&target);
    assert(engine.GetEventRouter().TargetCount() == 0);

    std::cout << "[PASS] test_engine_event_router_register" << std::endl;
}

void test_diagnostics_overlay_toggle_state() {
    // Ensure toggle works correctly
    bool initial = ui::DiagnosticsOverlay::IsEnabled();
    ui::DiagnosticsOverlay::Toggle();
    assert(ui::DiagnosticsOverlay::IsEnabled() != initial);
    ui::DiagnosticsOverlay::Toggle();
    assert(ui::DiagnosticsOverlay::IsEnabled() == initial);

    // Restore default state
    ui::DiagnosticsOverlay::SetEnabled(false);

    std::cout << "[PASS] test_diagnostics_overlay_toggle_state" << std::endl;
}
