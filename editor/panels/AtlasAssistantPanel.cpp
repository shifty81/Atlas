#include "AtlasAssistantPanel.h"

namespace atlas::editor {

void AtlasAssistantPanel::Draw() {
    // UI rendering handled by backend.
}

void AtlasAssistantPanel::SetContext(const std::string& context) {
    m_context = context;
}

std::string AtlasAssistantPanel::GetContext() const {
    return m_context;
}

void AtlasAssistantPanel::SubmitPrompt(const AssistantPrompt& prompt) {
    m_conversation.prompts.push_back(prompt);
}

const std::vector<AssistantPrompt>& AtlasAssistantPanel::Prompts() const {
    return m_conversation.prompts;
}

void AtlasAssistantPanel::AddSuggestion(const AssistantSuggestion& suggestion) {
    m_conversation.suggestions.push_back(suggestion);
}

const std::vector<AssistantSuggestion>& AtlasAssistantPanel::Suggestions() const {
    return m_conversation.suggestions;
}

size_t AtlasAssistantPanel::SuggestionCount() const {
    return m_conversation.suggestions.size();
}

void AtlasAssistantPanel::ApplySuggestion(size_t index) {
    if (index < m_conversation.suggestions.size()) {
        m_conversation.suggestions[index].applied = true;
    }
}

bool AtlasAssistantPanel::IsSuggestionApplied(size_t index) const {
    if (index < m_conversation.suggestions.size()) {
        return m_conversation.suggestions[index].applied;
    }
    return false;
}

void AtlasAssistantPanel::SetDiffPreview(const std::string& diff) {
    m_diffPreview = diff;
}

std::string AtlasAssistantPanel::GetDiffPreview() const {
    return m_diffPreview;
}

const AssistantConversation& AtlasAssistantPanel::GetConversation() const {
    return m_conversation;
}

void AtlasAssistantPanel::ClearConversation() {
    m_conversation.prompts.clear();
    m_conversation.suggestions.clear();
    m_context.clear();
    m_diffPreview.clear();
}

} // namespace atlas::editor
