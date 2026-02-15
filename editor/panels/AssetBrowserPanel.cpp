#include "AssetBrowserPanel.h"
#include <algorithm>
#include <sstream>
#include <filesystem>

namespace atlas::editor {

void AssetBrowserPanel::Draw() {
    Refresh();
}

void AssetBrowserPanel::Refresh() {
    m_entries.clear();

    auto allAssets = m_registry.GetAll();
    for (const auto& asset : allAssets) {
        AssetBrowserEntry entry;
        entry.id = asset.id;
        entry.path = asset.path;
        entry.version = asset.version;

        std::filesystem::path p(asset.path);
        entry.extension = p.extension().string();

        // Apply filter if set
        if (!m_filter.empty()) {
            bool matchesName = asset.id.find(m_filter) != std::string::npos;
            bool matchesExt = entry.extension.find(m_filter) != std::string::npos;
            if (!matchesName && !matchesExt) continue;
        }

        m_entries.push_back(std::move(entry));
    }

    // Sort based on current sort mode
    switch (m_sortMode) {
        case AssetSortMode::ByName:
            std::sort(m_entries.begin(), m_entries.end(),
                [](const AssetBrowserEntry& a, const AssetBrowserEntry& b) {
                    return a.id < b.id;
                });
            break;
        case AssetSortMode::ByType:
            std::sort(m_entries.begin(), m_entries.end(),
                [](const AssetBrowserEntry& a, const AssetBrowserEntry& b) {
                    return a.extension < b.extension;
                });
            break;
        case AssetSortMode::ByPath:
            std::sort(m_entries.begin(), m_entries.end(),
                [](const AssetBrowserEntry& a, const AssetBrowserEntry& b) {
                    return a.path < b.path;
                });
            break;
    }
}

std::string AssetBrowserPanel::Summary() const {
    std::ostringstream oss;
    oss << "Assets: " << m_entries.size();
    if (!m_filter.empty()) {
        oss << " (filter: " << m_filter << ")";
    }
    return oss.str();
}

}
