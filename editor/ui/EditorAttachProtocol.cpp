#include "EditorAttachProtocol.h"

namespace atlas::editor {

void EditorAttachProtocol::Init() {
    m_state = AttachState::Disconnected;
    m_config = AttachConfig{};
}

bool EditorAttachProtocol::Connect(const AttachConfig& config) {
    m_config = config;

    switch (config.mode) {
        case AttachMode::Standalone:
            m_state = AttachState::Connected;
            return true;

        case AttachMode::LiveClient:
            if (config.host.empty() || config.port == 0) {
                m_state = AttachState::Error;
                return false;
            }
            m_state = AttachState::Connecting;
            // Actual socket connection would happen here.
            // For now, transition directly to connected state.
            m_state = AttachState::Connected;
            return true;

        case AttachMode::HeadlessServer:
            if (config.host.empty() || config.port == 0) {
                m_state = AttachState::Error;
                return false;
            }
            m_state = AttachState::Connecting;
            m_state = AttachState::Connected;
            return true;

        case AttachMode::Replay:
            if (config.replayPath.empty()) {
                m_state = AttachState::Error;
                return false;
            }
            m_state = AttachState::Connected;
            return true;
    }

    m_state = AttachState::Error;
    return false;
}

void EditorAttachProtocol::Disconnect() {
    m_state = AttachState::Disconnected;
    m_config = AttachConfig{};
}

AttachMode EditorAttachProtocol::GetMode() const {
    return m_config.mode;
}

AttachState EditorAttachProtocol::GetState() const {
    return m_state;
}

std::string EditorAttachProtocol::TargetDescription() const {
    switch (m_config.mode) {
        case AttachMode::Standalone:
            return "Standalone";
        case AttachMode::LiveClient:
            return "LiveClient " + m_config.host + ":" + std::to_string(m_config.port);
        case AttachMode::HeadlessServer:
            return "HeadlessServer " + m_config.host + ":" + std::to_string(m_config.port);
        case AttachMode::Replay:
            return "Replay " + m_config.replayPath;
    }
    return "Unknown";
}

bool EditorAttachProtocol::IsConnected() const {
    return m_state == AttachState::Connected;
}

} // namespace atlas::editor
