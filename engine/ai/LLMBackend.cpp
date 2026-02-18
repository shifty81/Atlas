#include "LLMBackend.h"
#include <algorithm>

namespace atlas::ai {

// ============================================================
// OfflineLLMBackend
// ============================================================

LLMResponse OfflineLLMBackend::Complete(const LLMRequest& request) {
    ++m_callCount;

    LLMResponse resp;
    resp.requestId = request.requestId != 0 ? request.requestId : m_nextRequestId++;
    resp.success = true;

    // Search for a canned response whose prefix matches the prompt.
    for (const auto& [prefix, text] : m_responses) {
        if (request.prompt.rfind(prefix, 0) == 0) {
            resp.text = text;
            resp.tokensUsed = static_cast<uint32_t>(text.size() / 4 + 1);
            resp.latencyMs = 0.0f;
            return resp;
        }
    }

    // Default offline response.
    resp.text = "[offline] No response registered for prompt: " + request.prompt;
    resp.tokensUsed = 8;
    resp.latencyMs = 0.0f;
    return resp;
}

bool OfflineLLMBackend::IsAvailable() const { return true; }

std::string OfflineLLMBackend::Name() const { return "OfflineStub"; }

uint8_t OfflineLLMBackend::Capabilities() const {
    return static_cast<uint8_t>(LLMCapability::TextGeneration);
}

void OfflineLLMBackend::RegisterResponse(const std::string& prefix, const std::string& response) {
    m_responses[prefix] = response;
}

size_t OfflineLLMBackend::ResponseCount() const {
    return m_responses.size();
}

void OfflineLLMBackend::ClearResponses() {
    m_responses.clear();
}

uint64_t OfflineLLMBackend::CallCount() const {
    return m_callCount;
}

// ============================================================
// LLMBackendRegistry
// ============================================================

void LLMBackendRegistry::SetBackend(std::shared_ptr<ILLMBackend> backend) {
    m_backend = std::move(backend);
}

ILLMBackend& LLMBackendRegistry::GetBackend() {
    if (m_backend) return *m_backend;
    return m_offlineStub;
}

const ILLMBackend& LLMBackendRegistry::GetBackend() const {
    if (m_backend) return *m_backend;
    return m_offlineStub;
}

bool LLMBackendRegistry::HasRealBackend() const {
    return m_backend != nullptr;
}

LLMResponse LLMBackendRegistry::Complete(const LLMRequest& request) {
    LLMRequest req = request;
    if (req.requestId == 0) req.requestId = m_nextRequestId++;

    LLMResponse resp = GetBackend().Complete(req);
    m_history.push_back(resp);
    return resp;
}

LLMResponse LLMBackendRegistry::Complete(const std::string& prompt) {
    LLMRequest req;
    req.prompt = prompt;
    req.requestId = m_nextRequestId++;
    return Complete(req);
}

const std::vector<LLMResponse>& LLMBackendRegistry::ResponseHistory() const {
    return m_history;
}

size_t LLMBackendRegistry::RequestCount() const {
    return m_history.size();
}

void LLMBackendRegistry::ClearHistory() {
    m_history.clear();
}

} // namespace atlas::ai
