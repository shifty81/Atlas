#include "NetInspectorPanel.h"

namespace atlas::editor {

void NetInspectorPanel::Draw() {
    auto mode = m_net.Mode();
    switch (mode) {
        case net::NetMode::Standalone: m_snapshot.modeString = "Standalone"; break;
        case net::NetMode::Client:     m_snapshot.modeString = "Client"; break;
        case net::NetMode::Server:     m_snapshot.modeString = "Server"; break;
        case net::NetMode::P2P_Host:   m_snapshot.modeString = "P2P_Host"; break;
        case net::NetMode::P2P_Peer:   m_snapshot.modeString = "P2P_Peer"; break;
    }

    auto& peers = m_net.Peers();
    m_snapshot.peerCount = static_cast<uint32_t>(peers.size());
    m_snapshot.connectedCount = 0;
    float totalRtt = 0.0f;
    for (const auto& p : peers) {
        if (p.connected) {
            ++m_snapshot.connectedCount;
            totalRtt += p.rtt;
        }
    }
    m_snapshot.avgRtt = (m_snapshot.connectedCount > 0)
        ? totalRtt / static_cast<float>(m_snapshot.connectedCount)
        : 0.0f;
}

}
