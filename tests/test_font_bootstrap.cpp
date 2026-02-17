#include "../engine/ui/FontBootstrap.h"
#include <iostream>
#include <cassert>
#include <filesystem>

using namespace atlas::ui;

void test_font_bootstrap_initial_state() {
    FontBootstrap fb;
    assert(!fb.IsReady());
    assert(fb.GetDefaultFont() == kInvalidFont);
    assert(fb.GetBaseFontSize() == 18.0f);
    assert(fb.GetDPIScale() == 1.0f);
    std::cout << "[PASS] test_font_bootstrap_initial_state" << std::endl;
}

void test_font_bootstrap_init_missing_font_generates_fallback() {
    // Use a temp directory so we don't pollute the repo tree.
    auto tmpDir = std::filesystem::temp_directory_path() / "atlas_font_test";
    std::filesystem::remove_all(tmpDir);
    struct Cleanup {
        std::filesystem::path p;
        ~Cleanup() { std::filesystem::remove_all(p); }
    } cleanup{tmpDir};

    FontBootstrap fb;
    // Font file doesn't exist — fallback should be generated and Init succeeds.
    bool result = fb.Init(tmpDir.string(), 1.5f);
    assert(result);
    assert(fb.IsReady());
    assert(fb.GetDefaultFont() != kInvalidFont);
    assert(fb.GetDPIScale() == 1.5f);
    // Verify the fallback metrics file was created.
    assert(std::filesystem::exists(tmpDir / "fonts" / "builtin_fallback.json"));
    std::cout << "[PASS] test_font_bootstrap_init_missing_font_generates_fallback" << std::endl;
}

void test_font_bootstrap_init_unwritable_root() {
    FontBootstrap fb;
    // Path under /proc is not writable — fallback generation should fail.
    bool result = fb.Init("/proc/nonexistent_atlas_test", 1.0f);
    assert(!result);
    assert(!fb.IsReady());
    assert(fb.GetDefaultFont() == kInvalidFont);
    std::cout << "[PASS] test_font_bootstrap_init_unwritable_root" << std::endl;
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
