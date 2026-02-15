#include "InteractionDebugger.h"

namespace atlas::editor {

void InteractionDebugger::Record(const InteractionLogEntry& entry)
{
    log.push_back(entry);
}

void InteractionDebugger::Clear()
{
    log.clear();
}

size_t InteractionDebugger::EntryCount() const
{
    return log.size();
}

const InteractionLogEntry& InteractionDebugger::GetEntry(size_t index) const
{
    return log.at(index);
}

} // namespace atlas::editor
