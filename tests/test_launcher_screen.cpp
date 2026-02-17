#include "../editor/ui/LauncherScreen.h"
#include <iostream>
#include <cassert>

using namespace atlas::editor;

void test_launcher_initial_state() {
    LauncherScreen launcher;
    assert(launcher.Projects().empty());
    assert(launcher.SelectedIndex() == SIZE_MAX);
    assert(launcher.SelectedProject() == nullptr);
    assert(!launcher.IsProjectChosen());
    assert(!launcher.IsNewProjectRequested());
    assert(!launcher.IsQuitRequested());
    std::cout << "[PASS] test_launcher_initial_state" << std::endl;
}

void test_launcher_scan_nonexistent() {
    LauncherScreen launcher;
    launcher.ScanProjects("/nonexistent_directory_12345");
    assert(launcher.Projects().empty());
    std::cout << "[PASS] test_launcher_scan_nonexistent" << std::endl;
}

void test_launcher_select_invalid() {
    LauncherScreen launcher;
    launcher.SelectProject(0);
    assert(launcher.SelectedIndex() == SIZE_MAX);
    assert(launcher.SelectedProject() == nullptr);
    std::cout << "[PASS] test_launcher_select_invalid" << std::endl;
}

void test_launcher_confirm_no_selection() {
    LauncherScreen launcher;
    launcher.ConfirmSelection();
    assert(!launcher.IsProjectChosen());
    std::cout << "[PASS] test_launcher_confirm_no_selection" << std::endl;
}

void test_launcher_request_new_project() {
    LauncherScreen launcher;
    assert(!launcher.IsNewProjectRequested());
    launcher.RequestNewProject();
    assert(launcher.IsNewProjectRequested());
    std::cout << "[PASS] test_launcher_request_new_project" << std::endl;
}

void test_launcher_request_quit() {
    LauncherScreen launcher;
    assert(!launcher.IsQuitRequested());
    launcher.RequestQuit();
    assert(launcher.IsQuitRequested());
    std::cout << "[PASS] test_launcher_request_quit" << std::endl;
}
