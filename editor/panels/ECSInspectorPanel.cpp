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

    // Refresh state block info from WorldState
    m_stateBlocks.clear();
    if (m_worldState) {
        for (const auto& block : m_worldState->RegisteredBlocks()) {
            StateBlockEntry sbe;
            sbe.name = block.name;
            sbe.category = block.category;
            sbe.typeTag = block.typeTag;
            sbe.estimatedSize = block.estimatedSize;
            m_stateBlocks.push_back(std::move(sbe));
        }
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

std::string ECSInspectorPanel::StateBlockSummary() const {
    std::ostringstream oss;
    size_t simCount = 0, derivedCount = 0, presentationCount = 0, debugCount = 0;
    for (const auto& block : m_stateBlocks) {
        switch (block.category) {
            case sim::StateCategory::Simulated:    ++simCount; break;
            case sim::StateCategory::Derived:      ++derivedCount; break;
            case sim::StateCategory::Presentation: ++presentationCount; break;
            case sim::StateCategory::Debug:        ++debugCount; break;
        }
    }
    oss << "Blocks: " << m_stateBlocks.size()
        << " (Sim=" << simCount
        << " Derived=" << derivedCount
        << " Pres=" << presentationCount
        << " Debug=" << debugCount << ")";
    return oss.str();
}

}
