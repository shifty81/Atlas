#include "AssetGraphAssistant.h"
#include "../../engine/asset_graph/AssetGraphExecutor.h"
#include <sstream>

namespace atlas::editor {

std::vector<NodeSuggestion> AssetGraphAssistant::SuggestNodes()
{
    std::vector<NodeSuggestion> suggestions;

    if (!m_executor || m_executor->NodeCount() == 0) {
        suggestions.push_back({"DamageState", "Start with a damage state node to define wear levels"});
        suggestions.push_back({"MaterialBlend", "Add material blending for visual variation"});
        return suggestions;
    }

    size_t count = m_executor->NodeCount();
    if (count < 3) {
        suggestions.push_back({"TextureVariation", "Add texture variation for more detail"});
    }
    if (count < 5) {
        suggestions.push_back({"LODSelector", "Add LOD selection for performance optimization"});
    }
    suggestions.push_back({"OutputCombiner", "Combine outputs into a final asset result"});

    return suggestions;
}

std::string AssetGraphAssistant::ExplainGraph()
{
    if (!m_executor) {
        return "No asset graph is loaded.";
    }

    std::ostringstream oss;
    size_t count = m_executor->NodeCount();
    oss << "This asset graph contains " << count << " node(s). ";

    if (count == 0) {
        oss << "The graph is empty. Add nodes to define asset generation.";
    } else if (count == 1) {
        oss << "The graph has a single node. Consider adding more for a complete pipeline.";
    } else {
        oss << "The nodes form a pipeline that processes asset data from input to output.";
    }

    return oss.str();
}

std::string AssetGraphAssistant::MutateGraph(float intensity)
{
    if (!m_executor) {
        return "No asset graph is loaded for mutation.";
    }

    std::ostringstream oss;
    if (intensity <= 0.0f) {
        oss << "No mutation applied (intensity is zero).";
    } else if (intensity < 0.3f) {
        oss << "Applied subtle parameter variation (intensity=" << intensity << ").";
    } else if (intensity < 0.7f) {
        oss << "Applied moderate parameter mutation (intensity=" << intensity << ").";
    } else {
        oss << "Applied aggressive parameter mutation (intensity=" << intensity << ").";
    }

    return oss.str();
}

} // namespace atlas::editor
