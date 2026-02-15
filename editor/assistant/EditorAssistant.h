#pragma once
#include "../../engine/interaction/InteractionRouter.h"

namespace atlas::editor {

class EditorAssistant final
    : public interaction::InteractionRouter
{
public:
    interaction::Utterance Route(
        const interaction::Intent& intent,
        const interaction::Utterance& source
    ) override;
};

} // namespace atlas::editor
