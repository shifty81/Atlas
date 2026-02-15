#include "InteractionDebugPanel.h"

namespace atlas::editor {

void InteractionDebugPanel::Draw() {
    // Display state is already maintained by LogInteraction / ClearLog.
    // Queryable via GetLog(), LogSize().
    // A full UI backend would render a scrollable table of interactions
    // with columns: Speaker, Text, Intent, Confidence.
}

void InteractionDebugPanel::LogInteraction(const InteractionLogEntry& entry) {
    m_log.push_back(entry);
}

}
