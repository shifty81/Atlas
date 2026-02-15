#pragma once
#include "../../engine/interaction/Utterance.h"
#include "../../engine/interaction/Intent.h"
#include <vector>

namespace atlas::editor {

struct InteractionLogEntry {
    interaction::Utterance utterance;
    interaction::Intent intent;
};

class InteractionDebugger {
public:
    void Record(const InteractionLogEntry& entry);
    void Clear();
    size_t EntryCount() const;
    const InteractionLogEntry& GetEntry(size_t index) const;

private:
    std::vector<InteractionLogEntry> log;
};

} // namespace atlas::editor
