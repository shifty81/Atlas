#pragma once
#include "../ui/EditorPanel.h"
#include "../../engine/ecs/ECS.h"
#include <string>
#include <vector>

namespace atlas::editor {

struct InspectorEntry {
    ecs::EntityID entityID = 0;
    std::vector<std::string> componentNames;
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

private:
    ecs::World& m_world;
    ecs::EntityID m_selectedEntity = 0;
    std::vector<InspectorEntry> m_lastSnapshot;
};

}
