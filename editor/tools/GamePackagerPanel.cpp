#include "GamePackagerPanel.h"
#include <sstream>

namespace atlas::editor {

void GamePackagerPanel::Draw() {
    // Display state is maintained via Settings().
    // Queryable via Settings() and SettingsSummary().
    // A full UI backend would render target selector, mode selector,
    // output path field, option checkboxes, and a Build button
    // triggering: Validate → Cook → Strip → Bundle → Copy Runtime → Emit.
}

std::string GamePackagerPanel::SettingsSummary() const {
    std::ostringstream oss;
    oss << "Target: ";
    switch (m_settings.target) {
        case BuildTarget::Client: oss << "Client"; break;
        case BuildTarget::Server: oss << "Server"; break;
    }
    oss << ", Mode: ";
    switch (m_settings.mode) {
        case BuildMode::Debug:       oss << "Debug"; break;
        case BuildMode::Development: oss << "Development"; break;
        case BuildMode::Release:     oss << "Release"; break;
    }
    oss << ", Output: " << m_settings.outputPath;
    return oss.str();
}

production::PackageReport GamePackagerPanel::Build() {
    m_building = true;
    m_currentStage = "";
    m_currentProgress = 0.0f;

    // Convert panel settings to packager config
    production::PackageConfig config;
    config.target = (m_settings.target == BuildTarget::Client)
                    ? production::PackageTarget::Client
                    : production::PackageTarget::Server;

    switch (m_settings.mode) {
        case BuildMode::Debug:       config.profile = production::ProfileType::Debug; break;
        case BuildMode::Development: config.profile = production::ProfileType::Development; break;
        case BuildMode::Release:     config.profile = production::ProfileType::Release; break;
    }

    config.outputDir = m_settings.outputPath;
    config.sourceDir = m_settings.sourceDir;
    config.includeMods = m_settings.includeMods;
    config.stripEditorData = m_settings.stripEditorData;
    config.singleExecutable = m_settings.singleExe;

    // Wire up progress callback
    m_packager.SetProgressCallback(
        [this](const std::string& stage, float progress) {
            m_currentStage = stage;
            m_currentProgress = progress;
        });

    m_lastReport = m_packager.Package(config);
    m_building = false;
    return m_lastReport;
}

}
