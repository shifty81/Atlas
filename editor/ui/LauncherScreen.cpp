#include "LauncherScreen.h"
#include "../../engine/core/Logger.h"
#include <filesystem>

namespace atlas::editor {

void LauncherScreen::ScanProjects(const std::string& projectsDir) {
    m_projects.clear();
    m_selected = SIZE_MAX;
    m_projectChosen = false;

    if (!std::filesystem::is_directory(projectsDir)) {
        Logger::Warn("Projects directory not found: " + projectsDir);
        return;
    }

    for (const auto& entry : std::filesystem::directory_iterator(projectsDir)) {
        if (!entry.is_directory()) continue;

        bool hasProjectDescriptor = false;
        for (const auto& file : std::filesystem::directory_iterator(entry.path())) {
            if (file.is_regular_file() && file.path().extension() == ".atlas") {
                hasProjectDescriptor = true;
                break;
            }
        }
        if (!hasProjectDescriptor) continue;

        ProjectEntry pe;
        pe.name = entry.path().filename().string();
        pe.path = entry.path().string();
        // engineVersion and lastOpened would be read from the
        // .atlas JSON file when the schema parser is wired up.
        pe.engineVersion = "0.1.0";
        pe.lastOpened = "";
        m_projects.push_back(std::move(pe));
    }

    Logger::Info("Launcher: found " + std::to_string(m_projects.size()) +
                 " project(s) in " + projectsDir);
}

const std::vector<ProjectEntry>& LauncherScreen::Projects() const {
    return m_projects;
}

void LauncherScreen::SelectProject(size_t index) {
    if (index < m_projects.size()) {
        m_selected = index;
    }
}

size_t LauncherScreen::SelectedIndex() const {
    return m_selected;
}

const ProjectEntry* LauncherScreen::SelectedProject() const {
    if (m_selected < m_projects.size()) {
        return &m_projects[m_selected];
    }
    return nullptr;
}

bool LauncherScreen::IsProjectChosen() const {
    return m_projectChosen;
}

void LauncherScreen::ConfirmSelection() {
    if (m_selected < m_projects.size()) {
        m_projectChosen = true;
    }
}

bool LauncherScreen::IsNewProjectRequested() const {
    return m_newProjectRequested;
}

void LauncherScreen::RequestNewProject() {
    m_newProjectRequested = true;
}

bool LauncherScreen::IsQuitRequested() const {
    return m_quitRequested;
}

void LauncherScreen::RequestQuit() {
    m_quitRequested = true;
}

} // namespace atlas::editor
