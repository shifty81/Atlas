#pragma once
#include <string>
#include <vector>

namespace atlas::asset { class AssetGraphExecutor; }

namespace atlas::editor {

struct NodeSuggestion {
    std::string name;
    std::string reason;
};

class AssetGraphAssistant {
public:
    void SetExecutor(asset::AssetGraphExecutor* executor) { m_executor = executor; }

    std::vector<NodeSuggestion> SuggestNodes();
    std::string ExplainGraph();
    std::string MutateGraph(float intensity);

private:
    asset::AssetGraphExecutor* m_executor = nullptr;
};

} // namespace atlas::editor
