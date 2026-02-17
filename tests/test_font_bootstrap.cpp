#include "../engine/ui/FontBootstrap.h"
#include <iostream>
#include <cassert>

using namespace atlas::ui;

void test_font_bootstrap_initial_state() {
    FontBootstrap fb;
    assert(!fb.IsReady());
    assert(fb.GetDefaultFont() == kInvalidFont);
    assert(fb.GetBaseFontSize() == 18.0f);
    assert(fb.GetDPIScale() == 1.0f);
    std::cout << "[PASS] test_font_bootstrap_initial_state" << std::endl;
}

void test_font_bootstrap_init_missing_font() {
    FontBootstrap fb;
    // Font file doesn't exist in test environment — should return false
    bool result = fb.Init("nonexistent_asset_root", 1.5f);
    assert(!result);
    assert(!fb.IsReady());
    assert(fb.GetDefaultFont() == kInvalidFont);
    assert(fb.GetDPIScale() == 1.5f);
    std::cout << "[PASS] test_font_bootstrap_init_missing_font" << std::endl;
}

void test_font_bootstrap_shutdown() {
    FontBootstrap fb;
    fb.Shutdown();
    assert(!fb.IsReady());
    assert(fb.GetDefaultFont() == kInvalidFont);
    std::cout << "[PASS] test_font_bootstrap_shutdown" << std::endl;
}

void test_font_bootstrap_rebuild_noop() {
    FontBootstrap fb;
    // Should not crash when no font loaded
    fb.RebuildFontAtlas();
    assert(!fb.IsReady());
    std::cout << "[PASS] test_font_bootstrap_rebuild_noop" << std::endl;
}
