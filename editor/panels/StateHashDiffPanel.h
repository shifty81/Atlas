#pragma once
#include "../../engine/sim/StateHasher.h"
#include <string>
#include <vector>

namespace atlas::editor {

struct HashDiffEntry {
    uint64_t tick = 0;
    uint64_t localHash = 0;
    uint64_t remoteHash = 0;
    bool matches = true;
};

class StateHashDiffPanel {
public:
    void SetLocalHasher(const sim::StateHasher* local);
    void SetRemoteHasher(const sim::StateHasher* remote);
    void Refresh();

    const std::vector<HashDiffEntry>& Entries() const;
    int64_t FirstDivergenceTick() const;
    bool HasDivergence() const;
    std::string Summary() const;

private:
    const sim::StateHasher* m_local = nullptr;
    const sim::StateHasher* m_remote = nullptr;
    std::vector<HashDiffEntry> m_entries;
    int64_t m_firstDivergence = -1;
};

}
