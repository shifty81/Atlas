#include "../editor/panels/ECSInspectorPanel.h"
#include <iostream>
#include <cassert>

using namespace atlas::ecs;
using namespace atlas::editor;

struct InspTestPosition {
    float x = 0.0f;
    float y = 0.0f;
};

struct InspTestHealth {
    int hp = 100;
};

void test_inspector_empty_world() {
    World world;
    ECSInspectorPanel panel(world);
    panel.Draw();

    assert(panel.LastSnapshot().empty());
    assert(panel.Summary() == "Entities: 0, Components: 0");

    std::cout << "[PASS] test_inspector_empty_world" << std::endl;
}

void test_inspector_entities_with_components() {
    World world;
    EntityID e1 = world.CreateEntity();
    EntityID e2 = world.CreateEntity();

    world.AddComponent<InspTestPosition>(e1, {1.0f, 2.0f});
    world.AddComponent<InspTestHealth>(e1, {50});
    world.AddComponent<InspTestPosition>(e2, {3.0f, 4.0f});

    ECSInspectorPanel panel(world);
    panel.Draw();

    auto& snap = panel.LastSnapshot();
    assert(snap.size() == 2);
    assert(snap[0].entityID == e1);
    assert(snap[0].componentNames.size() == 2);
    assert(snap[1].entityID == e2);
    assert(snap[1].componentNames.size() == 1);

    auto summary = panel.Summary();
    assert(summary == "Entities: 2, Components: 3");

    std::cout << "[PASS] test_inspector_entities_with_components" << std::endl;
}

void test_inspector_select_entity() {
    World world;
    EntityID e1 = world.CreateEntity();

    ECSInspectorPanel panel(world);
    assert(panel.SelectedEntity() == 0);

    panel.SelectEntity(e1);
    assert(panel.SelectedEntity() == e1);

    std::cout << "[PASS] test_inspector_select_entity" << std::endl;
}

void test_inspector_refreshes_on_draw() {
    World world;
    ECSInspectorPanel panel(world);

    panel.Draw();
    assert(panel.LastSnapshot().empty());

    world.CreateEntity();
    panel.Draw();
    assert(panel.LastSnapshot().size() == 1);

    world.CreateEntity();
    world.CreateEntity();
    panel.Draw();
    assert(panel.LastSnapshot().size() == 3);

    std::cout << "[PASS] test_inspector_refreshes_on_draw" << std::endl;
}
