#include "EditorAssistant.h"

namespace atlas::editor {

using namespace interaction;

Utterance EditorAssistant::Route(
    const Intent& intent,
    const Utterance&)
{
    if (intent.name == "ExplainPerformance") {
        return {
            "This region is slow because a noise warp node "
            "is recalculated per LOD. Consider caching.",
            0, 0,
            { InteractionMode::Debug, true }
        };
    }

    if (intent.name == "ExplainGraphNode") {
        return {
            "This node modifies domain coordinates before "
            "sampling noise, increasing terrain variation.",
            0, 0,
            { InteractionMode::Debug, true }
        };
    }

    return {
        "I don't have an explanation for that yet.",
        0, 0,
        { InteractionMode::Debug, true }
    };
}

} // namespace atlas::editor
