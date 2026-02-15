#pragma once

namespace atlas::editor {

class AssetGraphAssistant {
public:
    void SuggestNodes();
    void ExplainGraph();
    void MutateGraph(float intensity);
};

} // namespace atlas::editor
