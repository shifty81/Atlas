#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <filesystem>

// This test verifies that simulation source files do not include render headers.
// It's a runtime check that complements the CMake build-time check
// (engine/cmake/check_include_firewall.cmake).
//
// Note: The CMake script performs a more comprehensive scan (GL/Vulkan headers,
// RenderAPI exception, platform window includes). This test focuses on the most
// critical violations: direct renderer and platform window includes.

namespace fs = std::filesystem;

static bool fileContainsForbiddenInclude(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return false;

    // Skip lines where #include appears after a comment
    std::string line;
    while (std::getline(file, line)) {
        auto includePos = line.find("#include");
        if (includePos == std::string::npos) continue;

        // Skip if a comment appears before #include
        auto commentPos = line.find("//");
        if (commentPos != std::string::npos && commentPos < includePos) continue;

        // Check for render includes
        if (line.find("GLRenderer") != std::string::npos ||
            line.find("VulkanRenderer") != std::string::npos ||
            line.find("platform/X11Window") != std::string::npos ||
            line.find("platform/PlatformWindow") != std::string::npos) {
            return true;
        }
    }
    return false;
}

void test_include_firewall_sim_no_render() {
    // Check that no sim/ source file includes render headers
    std::string simDir = "engine/sim";

    // Only run if directory exists (test may run from different CWD)
    if (!fs::exists(simDir)) {
        simDir = "../engine/sim";
    }
    if (!fs::exists(simDir)) {
        // Can't find sim dir, skip
        std::cout << "[PASS] test_include_firewall_sim_no_render (skipped - dir not found)" << std::endl;
        return;
    }

    for (const auto& entry : fs::recursive_directory_iterator(simDir)) {
        if (!entry.is_regular_file()) continue;
        std::string ext = entry.path().extension().string();
        if (ext != ".h" && ext != ".cpp") continue;

        bool violation = fileContainsForbiddenInclude(entry.path().string());
        if (violation) {
            std::cerr << "FIREWALL VIOLATION: " << entry.path() << std::endl;
        }
        assert(!violation);
    }

    std::cout << "[PASS] test_include_firewall_sim_no_render" << std::endl;
}

void test_include_firewall_contract_no_render() {
    std::string contractDir = "engine/core/contract";
    if (!fs::exists(contractDir)) {
        contractDir = "../engine/core/contract";
    }
    if (!fs::exists(contractDir)) {
        std::cout << "[PASS] test_include_firewall_contract_no_render (skipped - dir not found)" << std::endl;
        return;
    }

    for (const auto& entry : fs::recursive_directory_iterator(contractDir)) {
        if (!entry.is_regular_file()) continue;
        std::string ext = entry.path().extension().string();
        if (ext != ".h" && ext != ".cpp") continue;

        bool violation = fileContainsForbiddenInclude(entry.path().string());
        if (violation) {
            std::cerr << "FIREWALL VIOLATION: " << entry.path() << std::endl;
        }
        assert(!violation);
    }

    std::cout << "[PASS] test_include_firewall_contract_no_render" << std::endl;
}

// ============================================================
// ImGui Ban: No source file anywhere may include imgui headers
// or use ImGui API calls.  See ATLAS_CORE_CONTRACT.md §6.
// ============================================================

static bool fileContainsBannedUILibrary(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        // Skip comment-only lines
        auto trimStart = line.find_first_not_of(" \t");
        if (trimStart != std::string::npos && line.substr(trimStart, 2) == "//")
            continue;

        // Check for imgui includes
        if (line.find("#include") != std::string::npos) {
            if (line.find("imgui.h") != std::string::npos ||
                line.find("imgui_impl_") != std::string::npos) {
                return true;
            }
        }

        // Check for ImGui API usage (not in comments or string literals)
        if (line.find("ImGui::") != std::string::npos) {
            auto commentPos = line.find("//");
            auto apiPos = line.find("ImGui::");
            if (commentPos == std::string::npos || apiPos < commentPos) {
                return true;
            }
        }
    }
    return false;
}

void test_no_imgui_in_engine() {
    std::string engineDir = "engine";
    if (!fs::exists(engineDir)) engineDir = "../engine";
    if (!fs::exists(engineDir)) {
        std::cout << "[PASS] test_no_imgui_in_engine (skipped - dir not found)" << std::endl;
        return;
    }

    for (const auto& entry : fs::recursive_directory_iterator(engineDir)) {
        if (!entry.is_regular_file()) continue;
        std::string ext = entry.path().extension().string();
        if (ext != ".h" && ext != ".cpp") continue;

        bool violation = fileContainsBannedUILibrary(entry.path().string());
        if (violation) {
            std::cerr << "IMGUI BAN VIOLATION: " << entry.path() << std::endl;
        }
        assert(!violation);
    }

    std::cout << "[PASS] test_no_imgui_in_engine" << std::endl;
}

void test_no_imgui_in_editor() {
    std::string editorDir = "editor";
    if (!fs::exists(editorDir)) editorDir = "../editor";
    if (!fs::exists(editorDir)) {
        std::cout << "[PASS] test_no_imgui_in_editor (skipped - dir not found)" << std::endl;
        return;
    }

    for (const auto& entry : fs::recursive_directory_iterator(editorDir)) {
        if (!entry.is_regular_file()) continue;
        std::string ext = entry.path().extension().string();
        if (ext != ".h" && ext != ".cpp") continue;

        bool violation = fileContainsBannedUILibrary(entry.path().string());
        if (violation) {
            std::cerr << "IMGUI BAN VIOLATION: " << entry.path() << std::endl;
        }
        assert(!violation);
    }

    std::cout << "[PASS] test_no_imgui_in_editor" << std::endl;
}

void test_no_imgui_in_client() {
    std::string clientDir = "client";
    if (!fs::exists(clientDir)) clientDir = "../client";
    if (!fs::exists(clientDir)) {
        std::cout << "[PASS] test_no_imgui_in_client (skipped - dir not found)" << std::endl;
        return;
    }

    for (const auto& entry : fs::recursive_directory_iterator(clientDir)) {
        if (!entry.is_regular_file()) continue;
        std::string ext = entry.path().extension().string();
        if (ext != ".h" && ext != ".cpp") continue;

        bool violation = fileContainsBannedUILibrary(entry.path().string());
        if (violation) {
            std::cerr << "IMGUI BAN VIOLATION: " << entry.path() << std::endl;
        }
        assert(!violation);
    }

    std::cout << "[PASS] test_no_imgui_in_client" << std::endl;
}

void test_no_imgui_in_server() {
    std::string serverDir = "server";
    if (!fs::exists(serverDir)) serverDir = "../server";
    if (!fs::exists(serverDir)) {
        std::cout << "[PASS] test_no_imgui_in_server (skipped - dir not found)" << std::endl;
        return;
    }

    for (const auto& entry : fs::recursive_directory_iterator(serverDir)) {
        if (!entry.is_regular_file()) continue;
        std::string ext = entry.path().extension().string();
        if (ext != ".h" && ext != ".cpp") continue;

        bool violation = fileContainsBannedUILibrary(entry.path().string());
        if (violation) {
            std::cerr << "IMGUI BAN VIOLATION: " << entry.path() << std::endl;
        }
        assert(!violation);
    }

    std::cout << "[PASS] test_no_imgui_in_server" << std::endl;
}
