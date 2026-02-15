#include "VoiceCommandPanel.h"

namespace atlas::editor {

void VoiceCommandPanel::Draw() {
    // Display state is already maintained by SetListening / ProcessPhrase.
    // Queryable via IsListening(), LastPhrase(), LastResolvedIntent().
    // A full UI backend would render status indicator, phrase log, and
    // the list of registered voice commands here.
}

void VoiceCommandPanel::ProcessPhrase(const std::string& phrase) {
    m_lastPhrase = phrase;
    m_lastIntent.clear();

    auto* cmd = m_voiceRegistry.Match(phrase);
    if (cmd) {
        m_lastIntent = cmd->intentName;
        interaction::Intent intent;
        intent.name = cmd->intentName;
        intent.source = interaction::IntentSource::Voice;
        m_intentRegistry.Dispatch(intent);
    }
}

}
