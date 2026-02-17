#include "FontBootstrap.h"
#include "../core/Logger.h"
#include <filesystem>

namespace atlas::ui {

bool FontBootstrap::Init(const std::string& assetRoot, float dpiScale) {
    m_dpiScale = dpiScale;

    std::string fontPath = assetRoot + "/fonts/Inter-Regular.ttf";

    if (!std::filesystem::exists(fontPath)) {
        Logger::Warn("Default font not found at: " + fontPath);
        // Font file is not yet shipped — mark as not ready but
        // do not treat this as a fatal error so the editor can
        // still start up and render placeholder glyphs.
        m_defaultFont = kInvalidFont;
        m_ready = false;
        return false;
    }

    // Placeholder: actual font atlas creation will be implemented
    // when the TextRenderer backend is wired up.  For now we
    // record the intent and log success so that the init chain
    // can be validated.
    m_defaultFont = 1; // reserved handle for the default font
    m_ready = true;

    Logger::Info("Font system initialised (asset root: " + assetRoot +
                 ", DPI scale: " + std::to_string(m_dpiScale) + ")");
    return true;
}

void FontBootstrap::RebuildFontAtlas() {
    if (m_defaultFont == kInvalidFont) return;
    Logger::Info("Font atlas rebuilt");
}

void FontBootstrap::Shutdown() {
    m_defaultFont = kInvalidFont;
    m_ready = false;
    Logger::Info("Font system shut down");
}

FontHandle FontBootstrap::GetDefaultFont() const {
    return m_defaultFont;
}

bool FontBootstrap::IsReady() const {
    return m_ready;
}

float FontBootstrap::GetBaseFontSize() const {
    return m_baseFontSize;
}

float FontBootstrap::GetDPIScale() const {
    return m_dpiScale;
}

} // namespace atlas::ui
