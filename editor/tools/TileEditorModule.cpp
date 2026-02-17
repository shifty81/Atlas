#include "TileEditorModule.h"
#include "../../engine/core/Logger.h"

namespace atlas::editor {

std::string TileEditorModule::Name() const {
    return "Tile Editor";
}

void TileEditorModule::OnRegister() {
    // Create a default layer so the editor is immediately usable.
    if (m_tileMap.layers.empty()) {
        TileLayer layer;
        layer.name = "Default";
        layer.zIndex = 0;
        m_tileMap.layers.push_back(std::move(layer));
    }
    Logger::Info("TileEditorModule registered");
}

void TileEditorModule::OnUnregister() {
    Logger::Info("TileEditorModule unregistered");
}

void TileEditorModule::RegisterPanels() {
    // Panels: TilePalette, LayerStack, Properties, BrushSettings, Viewport
    Logger::Info("TileEditorModule: panels registered");
}

void TileEditorModule::RegisterMenus() {
    Logger::Info("TileEditorModule: menus registered");
}

void TileEditorModule::RegisterModes() {
    // Paint, Erase, Select, LayerEdit, RuleEdit
    Logger::Info("TileEditorModule: modes registered");
}

bool TileEditorModule::HandleInput(uint32_t /*keyCode*/, bool /*pressed*/) {
    return false; // not yet wired
}

void TileEditorModule::Update(float /*deltaTime*/) {
    // Tick logic (e.g. auto-tiling rule evaluation).
}

void TileEditorModule::Render() {
    // Tile rendering delegated to ITileRenderer (to be implemented).
}

// --- Tile-editor-specific API ---

void TileEditorModule::SetMode(TileEditorMode mode) {
    m_mode = mode;
}

TileEditorMode TileEditorModule::GetMode() const {
    return m_mode;
}

TileMap& TileEditorModule::GetTileMap() {
    return m_tileMap;
}

const TileMap& TileEditorModule::GetTileMap() const {
    return m_tileMap;
}

void TileEditorModule::SetSelectedTile(uint32_t tileAssetId) {
    m_selectedTile = tileAssetId;
}

uint32_t TileEditorModule::GetSelectedTile() const {
    return m_selectedTile;
}

void TileEditorModule::SetActiveLayer(size_t layerIndex) {
    if (layerIndex < m_tileMap.layers.size()) {
        m_activeLayer = layerIndex;
    }
}

size_t TileEditorModule::GetActiveLayer() const {
    return m_activeLayer;
}

void TileEditorModule::PaintTile(const GridCoord& coord) {
    if (m_activeLayer >= m_tileMap.layers.size()) return;
    if (m_selectedTile == 0) return;

    TileInstance inst;
    inst.tileAssetId = m_selectedTile;
    m_tileMap.layers[m_activeLayer].tiles[coord] = inst;
}

void TileEditorModule::EraseTile(const GridCoord& coord) {
    if (m_activeLayer >= m_tileMap.layers.size()) return;
    m_tileMap.layers[m_activeLayer].tiles.erase(coord);
}

} // namespace atlas::editor
