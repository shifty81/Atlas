#pragma once
#include "../ui/EditorPanel.h"
#include "../../engine/ecs/ECS.h"
#include "../../engine/sim/WorldState.h"
#include <string>
#include <vector>

namespace atlas::editor {

struct InspectorEntry {
    ecs::EntityID entityID = 0;
    std::vector<std::string> componentNames;
};

struct StateBlockEntry {
    std::string name;
    sim::StateCategory category = sim::StateCategory::Simulated;
    uint32_t typeTag = 0;
    size_t estimatedSize = 0;
};

class ECSInspectorPanel : public EditorPanel {
public:
    explicit ECSInspectorPanel(ecs::World& world) : m_world(world) {}

    const char* Name() const override { return "ECS Inspector"; }
    void Draw() override;

    void SelectEntity(ecs::EntityID id) { m_selectedEntity = id; }
    ecs::EntityID SelectedEntity() const { return m_selectedEntity; }

    const std::vector<InspectorEntry>& LastSnapshot() const { return m_lastSnapshot; }

    std::string Summary() const;

    // State block inspector
    void SetWorldState(const sim::WorldState* ws) { m_worldState = ws; }
    const std::vector<StateBlockEntry>& StateBlocks() const { return m_stateBlocks; }
    std::string StateBlockSummary() const;

private:
    ecs::World& m_world;
    ecs::EntityID m_selectedEntity = 0;
    std::vector<InspectorEntry> m_lastSnapshot;
    const sim::WorldState* m_worldState = nullptr;
    std::vector<StateBlockEntry> m_stateBlocks;
};

}
