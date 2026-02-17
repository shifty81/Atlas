#include "../editor/tools/TileEditorModule.h"
#include <iostream>
#include <cassert>

using namespace atlas::editor;

void test_tile_editor_name() {
    TileEditorModule mod;
    assert(mod.Name() == "Tile Editor");
    std::cout << "[PASS] test_tile_editor_name" << std::endl;
}

void test_tile_editor_register() {
    TileEditorModule mod;
    mod.OnRegister();
    // Should create a default layer
    assert(!mod.GetTileMap().layers.empty());
    assert(mod.GetTileMap().layers[0].name == "Default");
    std::cout << "[PASS] test_tile_editor_register" << std::endl;
}

void test_tile_editor_modes() {
    TileEditorModule mod;
    assert(mod.GetMode() == TileEditorMode::Paint);
    mod.SetMode(TileEditorMode::Erase);
    assert(mod.GetMode() == TileEditorMode::Erase);
    mod.SetMode(TileEditorMode::Select);
    assert(mod.GetMode() == TileEditorMode::Select);
    std::cout << "[PASS] test_tile_editor_modes" << std::endl;
}

void test_tile_editor_paint() {
    TileEditorModule mod;
    mod.OnRegister();
    mod.SetSelectedTile(42);
    assert(mod.GetSelectedTile() == 42);

    mod.PaintTile({3, 5});
    auto& layer = mod.GetTileMap().layers[0];
    auto it = layer.tiles.find({3, 5});
    assert(it != layer.tiles.end());
    assert(it->second.tileAssetId == 42);
    std::cout << "[PASS] test_tile_editor_paint" << std::endl;
}

void test_tile_editor_erase() {
    TileEditorModule mod;
    mod.OnRegister();
    mod.SetSelectedTile(1);
    mod.PaintTile({1, 1});
    assert(mod.GetTileMap().layers[0].tiles.count({1, 1}) == 1);

    mod.EraseTile({1, 1});
    assert(mod.GetTileMap().layers[0].tiles.count({1, 1}) == 0);
    std::cout << "[PASS] test_tile_editor_erase" << std::endl;
}

void test_tile_editor_paint_no_tile_selected() {
    TileEditorModule mod;
    mod.OnRegister();
    // selectedTile is 0 by default — painting should be a no-op
    mod.PaintTile({2, 2});
    assert(mod.GetTileMap().layers[0].tiles.empty());
    std::cout << "[PASS] test_tile_editor_paint_no_tile_selected" << std::endl;
}

void test_tile_editor_active_layer() {
    TileEditorModule mod;
    mod.OnRegister();
    assert(mod.GetActiveLayer() == 0);

    // Add a second layer
    TileLayer layer2;
    layer2.name = "Layer2";
    layer2.zIndex = 1;
    mod.GetTileMap().layers.push_back(std::move(layer2));

    mod.SetActiveLayer(1);
    assert(mod.GetActiveLayer() == 1);

    // Out-of-bounds should not change
    mod.SetActiveLayer(99);
    assert(mod.GetActiveLayer() == 1);
    std::cout << "[PASS] test_tile_editor_active_layer" << std::endl;
}

void test_tile_editor_unregister() {
    TileEditorModule mod;
    mod.OnRegister();
    mod.OnUnregister();
    // Should not crash
    std::cout << "[PASS] test_tile_editor_unregister" << std::endl;
}

void test_tile_editor_grid_coord_hash() {
    // Verify that GridCoord hash works for positive and negative coords
    std::hash<GridCoord> h;
    GridCoord a{0, 0};
    GridCoord b{1, 0};
    GridCoord c{0, 1};
    GridCoord d{-1, -1};

    // Different coordinates should (very likely) produce different hashes
    assert(h(a) != h(b) || (a == b));
    assert(h(a) != h(c) || (a == c));
    assert(h(a) != h(d) || (a == d));
    std::cout << "[PASS] test_tile_editor_grid_coord_hash" << std::endl;
}
