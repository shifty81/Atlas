#include "NetHardening.h"

namespace atlas::net {

void NetHardening::Configure(const NetHardeningConfig& config) {
    m_config = config;
}

const NetHardeningConfig& NetHardening::Config() const {
    return m_config;
}

void NetHardening::Update(float deltaTimeMs) {
    m_secondAccumulator += deltaTimeMs;
    if (m_secondAccumulator >= 1000.0f) {
        m_secondAccumulator -= 1000.0f;
        m_bytesSentThisSecond = 0.0f;
    }

    switch (m_stats.state) {
        case ConnectionState::Connecting:
            m_timeSinceConnectStart += deltaTimeMs;
            if (m_config.connectionTimeoutMs > 0 &&
                m_timeSinceConnectStart >= static_cast<float>(m_config.connectionTimeoutMs)) {
                HandleTimeout();
            }
            break;

        case ConnectionState::Connected:
            if (m_config.heartbeatIntervalMs > 0) {
                m_timeSinceLastHeartbeat += deltaTimeMs;
                float threshold = static_cast<float>(m_config.heartbeatIntervalMs) *
                                  static_cast<float>(m_config.heartbeatMissThreshold);
                if (m_timeSinceLastHeartbeat >= threshold) {
                    HandleTimeout();
                }
            }
            break;

        case ConnectionState::Reconnecting:
            m_timeSinceLastReconnect += deltaTimeMs;
            if (m_timeSinceLastReconnect >= static_cast<float>(m_config.reconnectDelayMs)) {
                AttemptReconnect();
            }
            break;

        default:
            break;
    }
}

void NetHardening::Connect() {
    m_timeSinceConnectStart = 0.0f;
    m_reconnectAttempts = 0;
    SetState(ConnectionState::Connecting);
}

void NetHardening::Disconnect() {
    SetState(ConnectionState::Disconnected);
}

void NetHardening::RecordBytesSent(uint32_t bytes) {
    m_stats.bytesSent += bytes;
    m_bytesSentThisSecond += static_cast<float>(bytes);
}

void NetHardening::RecordBytesReceived(uint32_t bytes) {
    m_stats.bytesReceived += bytes;
}

void NetHardening::RecordPacketSent() {
    m_stats.packetsSent++;
}

void NetHardening::RecordPacketReceived() {
    m_stats.packetsReceived++;

    // Receiving a packet while connecting means we're connected
    if (m_stats.state == ConnectionState::Connecting) {
        SetState(ConnectionState::Connected);
    }
}

void NetHardening::RecordPacketDropped() {
    m_stats.packetsDropped++;
}

void NetHardening::RecordRtt(float rttMs) {
    if (rttMs > m_stats.peakRttMs) {
        m_stats.peakRttMs = rttMs;
    }
    // Exponential moving average
    if (m_stats.averageRttMs == 0.0f) {
        m_stats.averageRttMs = rttMs;
    } else {
        m_stats.averageRttMs = m_stats.averageRttMs * 0.9f + rttMs * 0.1f;
    }
}

void NetHardening::RecordHeartbeat() {
    m_timeSinceLastHeartbeat = 0.0f;
}

bool NetHardening::CanSendBytes(uint32_t bytes) const {
    if (m_config.maxBandwidthBytesPerSec == 0) return true;
    return (m_bytesSentThisSecond + static_cast<float>(bytes)) <=
           static_cast<float>(m_config.maxBandwidthBytesPerSec);
}

bool NetHardening::IsPacketSizeValid(uint32_t bytes) const {
    return bytes <= m_config.maxPacketSize;
}

ConnectionState NetHardening::State() const {
    return m_stats.state;
}

const ConnectionStats& NetHardening::Stats() const {
    return m_stats;
}

void NetHardening::SetStateCallback(StateCallback cb) {
    m_stateCallback = std::move(cb);
}

void NetHardening::ResetStats() {
    ConnectionState currentState = m_stats.state;
    m_stats = ConnectionStats();
    m_stats.state = currentState;
}

void NetHardening::SetState(ConnectionState newState) {
    ConnectionState oldState = m_stats.state;
    if (oldState == newState) return;
    m_stats.state = newState;
    if (m_stateCallback) {
        m_stateCallback(oldState, newState);
    }
}

void NetHardening::HandleTimeout() {
    if (m_config.maxReconnectAttempts > 0 && m_reconnectAttempts < m_config.maxReconnectAttempts) {
        m_timeSinceLastReconnect = 0.0f;
        SetState(ConnectionState::Reconnecting);
    } else {
        SetState(ConnectionState::TimedOut);
    }
}

void NetHardening::AttemptReconnect() {
    m_reconnectAttempts++;
    m_stats.reconnectCount = m_reconnectAttempts;
    m_timeSinceLastReconnect = 0.0f;

    if (m_reconnectAttempts >= m_config.maxReconnectAttempts) {
        SetState(ConnectionState::TimedOut);
    } else {
        // Transition back to connecting to retry
        m_timeSinceConnectStart = 0.0f;
        SetState(ConnectionState::Connecting);
    }
}

}
