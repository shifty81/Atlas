#pragma once
#include <cstdint>
#include <string>

namespace atlas::editor {

/// Attach mode describing how the editor connects to a running session.
enum class AttachMode : uint8_t {
    Standalone,     ///< Editor launches its own runtime instance
    LiveClient,     ///< Editor attaches to a running client process
    HeadlessServer, ///< Editor attaches to a dedicated server over the network
    Replay          ///< Editor opens a replay file for offline inspection
};

/// Current state of the attach connection.
enum class AttachState : uint8_t {
    Disconnected,
    Connecting,
    Connected,
    Error
};

/// Configuration for an attach request.
struct AttachConfig {
    AttachMode mode = AttachMode::Standalone;
    std::string host;         ///< Network address (LiveClient / HeadlessServer)
    uint16_t   port = 0;     ///< Network port
    std::string replayPath;  ///< File path for Replay mode
};

/// Manages the editor's attachment to a target runtime process.
/// The attach protocol allows the editor to observe and (with sufficient
/// permissions) mutate the state of a live client, headless server, or
/// replay session.
class EditorAttachProtocol {
public:
    void Init();

    /// Begin an attach attempt with the given configuration.
    bool Connect(const AttachConfig& config);

    /// Disconnect from the current target.
    void Disconnect();

    AttachMode  GetMode() const;
    AttachState GetState() const;

    /// Human-readable description of the current attach target.
    std::string TargetDescription() const;

    /// Returns true when the editor has established a valid connection
    /// (or is in standalone mode, which is always "connected").
    bool IsConnected() const;

private:
    AttachConfig m_config;
    AttachState  m_state = AttachState::Disconnected;
};

} // namespace atlas::editor
