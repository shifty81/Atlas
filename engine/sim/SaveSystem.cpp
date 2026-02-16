#include "SaveSystem.h"
#include "StateHasher.h"
#include <fstream>
#include <cstring>

namespace atlas::sim {

SaveResult SaveSystem::Save(const std::string& path,
                            uint64_t tick,
                            uint32_t tickRate,
                            uint32_t seed,
                            const std::vector<uint8_t>& ecsData,
                            const std::vector<uint8_t>& auxData,
                            const std::string& metadata) {
    // Compute deterministic hash over all saved state.
    uint64_t hash = StateHasher::HashCombine(0, ecsData.data(), ecsData.size());
    if (!auxData.empty()) {
        hash = StateHasher::HashCombine(hash, auxData.data(), auxData.size());
    }

    SaveHeader header;
    header.saveTick = tick;
    header.tickRate = tickRate;
    header.seed = seed;
    header.stateHash = hash;
    header.ecsDataSize = static_cast<uint32_t>(ecsData.size());
    header.auxDataSize = static_cast<uint32_t>(auxData.size());
    header.metadataSize = static_cast<uint32_t>(metadata.size());

    std::ofstream out(path, std::ios::binary);
    if (!out.is_open()) {
        return SaveResult::IOError;
    }

    out.write(reinterpret_cast<const char*>(&header), sizeof(header));
    if (!ecsData.empty()) {
        out.write(reinterpret_cast<const char*>(ecsData.data()),
                  static_cast<std::streamsize>(ecsData.size()));
    }
    if (!auxData.empty()) {
        out.write(reinterpret_cast<const char*>(auxData.data()),
                  static_cast<std::streamsize>(auxData.size()));
    }
    if (!metadata.empty()) {
        out.write(metadata.data(),
                  static_cast<std::streamsize>(metadata.size()));
    }

    if (!out.good()) {
        return SaveResult::IOError;
    }

    return SaveResult::Success;
}

SaveResult SaveSystem::Load(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in.is_open()) {
        return SaveResult::FileNotFound;
    }

    SaveHeader header;
    in.read(reinterpret_cast<char*>(&header), sizeof(header));
    if (!in.good()) {
        return SaveResult::InvalidFormat;
    }

    if (header.magic != 0x41534156) {
        return SaveResult::InvalidFormat;
    }

    if (header.version != 1) {
        return SaveResult::VersionMismatch;
    }

    // Sanity check sizes to prevent malicious allocations.
    static constexpr uint32_t kMaxDataSize = 256 * 1024 * 1024;  // 256 MB
    if (header.ecsDataSize > kMaxDataSize ||
        header.auxDataSize > kMaxDataSize ||
        header.metadataSize > kMaxDataSize) {
        return SaveResult::InvalidFormat;
    }

    std::vector<uint8_t> ecsData(header.ecsDataSize);
    if (header.ecsDataSize > 0) {
        in.read(reinterpret_cast<char*>(ecsData.data()),
                static_cast<std::streamsize>(header.ecsDataSize));
    }

    std::vector<uint8_t> auxData(header.auxDataSize);
    if (header.auxDataSize > 0) {
        in.read(reinterpret_cast<char*>(auxData.data()),
                static_cast<std::streamsize>(header.auxDataSize));
    }

    std::string metadata(header.metadataSize, '\0');
    if (header.metadataSize > 0) {
        in.read(metadata.data(),
                static_cast<std::streamsize>(header.metadataSize));
    }

    if (!in.good() && !in.eof()) {
        return SaveResult::IOError;
    }

    // Verify hash integrity.
    uint64_t hash = StateHasher::HashCombine(0, ecsData.data(), ecsData.size());
    if (!auxData.empty()) {
        hash = StateHasher::HashCombine(hash, auxData.data(), auxData.size());
    }

    if (hash != header.stateHash) {
        return SaveResult::HashMismatch;
    }

    m_header = header;
    m_ecsData = std::move(ecsData);
    m_auxData = std::move(auxData);
    m_metadata = std::move(metadata);

    return SaveResult::Success;
}

SaveResult SaveSystem::Validate(const std::string& path) const {
    std::ifstream in(path, std::ios::binary);
    if (!in.is_open()) {
        return SaveResult::FileNotFound;
    }

    SaveHeader header;
    in.read(reinterpret_cast<char*>(&header), sizeof(header));
    if (!in.good()) {
        return SaveResult::InvalidFormat;
    }

    if (header.magic != 0x41534156) {
        return SaveResult::InvalidFormat;
    }

    if (header.version != 1) {
        return SaveResult::VersionMismatch;
    }

    return SaveResult::Success;
}

const SaveHeader& SaveSystem::Header() const {
    return m_header;
}

const std::vector<uint8_t>& SaveSystem::ECSData() const {
    return m_ecsData;
}

const std::vector<uint8_t>& SaveSystem::AuxData() const {
    return m_auxData;
}

const std::string& SaveSystem::Metadata() const {
    return m_metadata;
}

void SaveSystem::Clear() {
    m_header = SaveHeader{};
    m_ecsData.clear();
    m_auxData.clear();
    m_metadata.clear();
}

}  // namespace atlas::sim
