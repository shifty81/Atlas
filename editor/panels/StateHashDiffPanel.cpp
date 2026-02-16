#include "StateHashDiffPanel.h"
#include <sstream>
#include <cstdio>

namespace atlas::editor {

void StateHashDiffPanel::SetLocalHasher(const sim::StateHasher* local) {
    m_local = local;
}

void StateHashDiffPanel::SetRemoteHasher(const sim::StateHasher* remote) {
    m_remote = remote;
}

void StateHashDiffPanel::Refresh() {
    m_entries.clear();
    m_firstDivergence = -1;

    if (!m_local || !m_remote) return;

    const auto& localHist = m_local->History();
    const auto& remoteHist = m_remote->History();

    size_t count = localHist.size();
    if (remoteHist.size() < count) count = remoteHist.size();

    for (size_t i = 0; i < count; ++i) {
        HashDiffEntry entry;
        entry.tick = localHist[i].tick;
        entry.localHash = localHist[i].hash;
        entry.remoteHash = remoteHist[i].hash;
        entry.matches = (entry.localHash == entry.remoteHash);
        m_entries.push_back(entry);

        if (!entry.matches && m_firstDivergence < 0) {
            m_firstDivergence = static_cast<int64_t>(entry.tick);
        }
    }
}

const std::vector<HashDiffEntry>& StateHashDiffPanel::Entries() const {
    return m_entries;
}

int64_t StateHashDiffPanel::FirstDivergenceTick() const {
    return m_firstDivergence;
}

bool StateHashDiffPanel::HasDivergence() const {
    return m_firstDivergence >= 0;
}

std::string StateHashDiffPanel::Summary() const {
    if (!HasDivergence()) {
        return "No divergence";
    }

    // Find the divergent entry
    for (const auto& e : m_entries) {
        if (!e.matches) {
            char buf[128];
            std::snprintf(buf, sizeof(buf),
                "Divergence at tick %llu: local=0x%llX remote=0x%llX",
                (unsigned long long)e.tick,
                (unsigned long long)e.localHash,
                (unsigned long long)e.remoteHash);
            return std::string(buf);
        }
    }

    return "No divergence";
}

}
