#include "AssetRegistry.h"

namespace atlas::asset {

void AssetRegistry::Scan(const std::string& root) {
    if (!std::filesystem::exists(root)) return;

    for (const auto& p : std::filesystem::recursive_directory_iterator(root)) {
        if (p.path().extension() == ".atlas" || p.path().extension() == ".atlasb") {
            AssetEntry entry;
            entry.id = p.path().stem().string();
            entry.path = p.path().string();
            entry.version = 1;

            m_assets[entry.id] = entry;
            m_timestamps[entry.path] = std::filesystem::last_write_time(p);
        }
    }
}

const AssetEntry* AssetRegistry::Get(const std::string& id) const {
    auto it = m_assets.find(id);
    return it != m_assets.end() ? &it->second : nullptr;
}

std::vector<AssetEntry> AssetRegistry::GetAll() const {
    std::vector<AssetEntry> result;
    result.reserve(m_assets.size());
    for (const auto& [id, entry] : m_assets) {
        result.push_back(entry);
    }
    return result;
}

void AssetRegistry::SetReloadCallback(ReloadCallback cb) {
    m_onReload = std::move(cb);
}

void AssetRegistry::PollHotReload() {
    for (auto& [id, asset] : m_assets) {
        if (!std::filesystem::exists(asset.path)) continue;

        auto now = std::filesystem::last_write_time(asset.path);
        if (now != m_timestamps[asset.path]) {
            m_timestamps[asset.path] = now;
            asset.version++;
            if (m_onReload) m_onReload(asset);
        }
    }
}

size_t AssetRegistry::Count() const {
    return m_assets.size();
}

}
