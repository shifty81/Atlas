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

}
