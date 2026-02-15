#include "ECSInspectorPanel.h"
#include <sstream>

namespace atlas::editor {

void ECSInspectorPanel::Draw() {
    m_lastSnapshot.clear();

    auto entities = m_world.GetEntities();
    for (auto eid : entities) {
        InspectorEntry entry;
        entry.entityID = eid;

        auto types = m_world.GetComponentTypes(eid);
        for (const auto& ti : types) {
            // Demangle type name: use raw name as a best-effort label
            entry.componentNames.push_back(ti.name());
        }

        m_lastSnapshot.push_back(std::move(entry));
    }
}

std::string ECSInspectorPanel::Summary() const {
    std::ostringstream oss;
    oss << "Entities: " << m_lastSnapshot.size();
    size_t totalComponents = 0;
    for (const auto& entry : m_lastSnapshot) {
        totalComponents += entry.componentNames.size();
    }
    oss << ", Components: " << totalComponents;
    return oss.str();
}

}
