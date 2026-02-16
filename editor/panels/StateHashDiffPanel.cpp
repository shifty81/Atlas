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

void StateHashDiffPanel::SetComponentBreakdown(const ComponentHashBreakdown& breakdown) {
    m_componentBreakdown = breakdown;
    m_hasComponentBreakdown = true;
}

const ComponentHashBreakdown& StateHashDiffPanel::GetComponentBreakdown() const {
    return m_componentBreakdown;
}

bool StateHashDiffPanel::HasComponentBreakdown() const {
    return m_hasComponentBreakdown;
}

std::vector<std::string> StateHashDiffPanel::DivergentComponents() const {
    std::vector<std::string> result;
    if (!m_hasComponentBreakdown) return result;
    for (const auto& c : m_componentBreakdown.components) {
        if (!c.matches) {
            result.push_back(c.name);
        }
    }
    return result;
}

HashLadderComparison StateHashDiffPanel::CompareHashLadders(
        const sim::StateHasher& a,
        const sim::StateHasher& b) const {
    HashLadderComparison result;

    const auto& ah = a.History();
    const auto& bh = b.History();
    size_t count = std::min(ah.size(), bh.size());
    result.totalCount = count;
    result.matchCount = 0;

    for (size_t i = 0; i < count; ++i) {
        HashDiffEntry entry;
        entry.tick = ah[i].tick;
        entry.localHash = ah[i].hash;
        entry.remoteHash = bh[i].hash;
        entry.matches = (entry.localHash == entry.remoteHash);
        result.entries.push_back(entry);

        if (entry.matches) {
            ++result.matchCount;
        } else if (result.firstDivergenceTick < 0) {
            result.firstDivergenceTick = static_cast<int64_t>(entry.tick);
        }
    }

    result.matchPercentage = (count > 0)
        ? (static_cast<double>(result.matchCount) / static_cast<double>(count)) * 100.0
        : 100.0;

    return result;
}

DivergenceDetail StateHashDiffPanel::GetDivergenceDetail() const {
    DivergenceDetail detail;

    if (!HasDivergence()) {
        detail.summary = "No divergence";
        return detail;
    }

    // Find first non-matching entry
    for (const auto& e : m_entries) {
        if (!e.matches) {
            detail.tick = static_cast<int64_t>(e.tick);
            detail.localHash = e.localHash;
            detail.remoteHash = e.remoteHash;
            break;
        }
    }

    // Include component breakdown if available at that tick
    if (m_hasComponentBreakdown &&
        static_cast<int64_t>(m_componentBreakdown.tick) == detail.tick) {
        for (const auto& c : m_componentBreakdown.components) {
            if (!c.matches) {
                detail.divergentComponents.push_back(c);
            }
        }
    }

    char buf[256];
    std::snprintf(buf, sizeof(buf),
        "Divergence at tick %lld: local=0x%llX remote=0x%llX, %zu divergent component(s)",
        (long long)detail.tick,
        (unsigned long long)detail.localHash,
        (unsigned long long)detail.remoteHash,
        detail.divergentComponents.size());
    detail.summary = buf;

    return detail;
}

}
