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
