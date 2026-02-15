#include "ProjectPickerPanel.h"
#include <filesystem>

namespace atlas::editor {

void ProjectPickerPanel::Draw() {
    // Display state is already maintained by OpenProject / AddRecentProject.
    // Queryable via HasProjectSelected(), SelectedProjectPath(), RecentProjects().
    // A full UI backend would render the project picker modal with
    // an "Open Project" button, recent project list, and Exit button.
}

void ProjectPickerPanel::AddRecentProject(const std::string& path) {
    for (const auto& p : m_recentProjects) {
        if (p == path) return;
    }
    m_recentProjects.push_back(path);
}

bool ProjectPickerPanel::OpenProject(const std::string& atlasFilePath) {
    if (!std::filesystem::exists(atlasFilePath)) {
        return false;
    }
    if (!m_projectManager.Load(atlasFilePath)) {
        return false;
    }
    m_selectedPath = atlasFilePath;
    m_projectSelected = true;
    AddRecentProject(atlasFilePath);
    return true;
}

}
