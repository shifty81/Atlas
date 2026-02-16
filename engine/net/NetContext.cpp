#include "NetContext.h"
#include "../ecs/ECS.h"
#include <algorithm>
#include <cstring>

namespace atlas::net {

void NetContext::Init(NetMode mode) {
    m_mode = mode;
    m_peers.clear();
    m_snapshots.clear();
    m_inputHistory.clear();
    m_nextPeerID = 1;
    while (!m_outgoing.empty()) m_outgoing.pop();
    while (!m_incoming.empty()) m_incoming.pop();
}

void NetContext::Shutdown() {
    m_peers.clear();
    m_snapshots.clear();
    m_inputHistory.clear();
    while (!m_outgoing.empty()) m_outgoing.pop();
    while (!m_incoming.empty()) m_incoming.pop();
    m_mode = NetMode::Standalone;
}

void NetContext::Poll() {
    // In local/loopback mode, move outgoing packets to incoming
    // This enables testing without real sockets
    while (!m_outgoing.empty()) {
        m_incoming.push(m_outgoing.front().packet);
        m_outgoing.pop();
    }
}

void NetContext::Send(uint32_t peerID, const Packet& pkt) {
    QueuedPacket qp;
    qp.destPeerID = peerID;
    qp.packet = pkt;
    m_outgoing.push(qp);
}

void NetContext::Broadcast(const Packet& pkt) {
    QueuedPacket qp;
    qp.destPeerID = 0; // 0 = broadcast
    qp.packet = pkt;
    m_outgoing.push(qp);
}

void NetContext::Flush() {
    // In a real implementation, this would push buffered data to sockets
    // For local mode, Poll() already handles the transfer
}

NetMode NetContext::Mode() const {
    return m_mode;
}

const std::vector<NetPeer>& NetContext::Peers() const {
    return m_peers;
}

bool NetContext::IsAuthority() const {
    return m_mode == NetMode::Server || m_mode == NetMode::P2P_Host;
}

uint32_t NetContext::AddPeer() {
    NetPeer peer;
    peer.id = m_nextPeerID++;
    peer.rtt = 0.0f;
    peer.connected = true;
    m_peers.push_back(peer);
    return peer.id;
}

void NetContext::RemovePeer(uint32_t peerID) {
    m_peers.erase(
        std::remove_if(m_peers.begin(), m_peers.end(),
            [peerID](const NetPeer& p) { return p.id == peerID; }),
        m_peers.end()
    );
}

bool NetContext::Receive(Packet& outPkt) {
    if (m_incoming.empty()) return false;
    outPkt = m_incoming.front();
    m_incoming.pop();
    return true;
}

void NetContext::SetWorld(ecs::World* world) {
    m_world = world;
}

void NetContext::RecordInput(const InputFrame& frame) {
    m_inputHistory.push_back(frame);
}

const std::vector<InputFrame>& NetContext::RecordedInputs() const {
    return m_inputHistory;
}

void NetContext::SetInputApplyCallback(std::function<void(const InputFrame&)> cb) {
    m_inputApplyCallback = std::move(cb);
}

void NetContext::SaveSnapshot(uint32_t tick) {
    WorldSnapshot snap;
    snap.tick = tick;
    if (m_world) {
        snap.ecsState = m_world->Serialize();
    }
    m_snapshots.push_back(std::move(snap));
}

void NetContext::RollbackTo(uint32_t tick) {
    // Find the snapshot for the requested tick
    const WorldSnapshot* target = nullptr;
    for (const auto& snap : m_snapshots) {
        if (snap.tick == tick) {
            target = &snap;
            break;
        }
    }

    if (target && m_world && !target->ecsState.empty()) {
        m_world->Deserialize(target->ecsState);
    }

    // Remove snapshots after the rollback tick
    m_snapshots.erase(
        std::remove_if(m_snapshots.begin(), m_snapshots.end(),
            [tick](const WorldSnapshot& s) { return s.tick > tick; }),
        m_snapshots.end()
    );
}

void NetContext::ReplayFrom(uint32_t tick) {
    // Replay recorded input frames from the given tick onward
    if (!m_world) return;

    for (const auto& frame : m_inputHistory) {
        if (frame.tick >= tick) {
            // Apply the input frame to the world before ticking
            if (m_inputApplyCallback) {
                m_inputApplyCallback(frame);
            }
            m_world->Update(1.0f / 60.0f);
        }
    }
}

const std::vector<WorldSnapshot>& NetContext::Snapshots() const {
    return m_snapshots;
}

void NetContext::BroadcastSaveTick(uint32_t tick, uint64_t stateHash) {
    m_lastSaveTick = tick;
    m_lastSaveHash = stateHash;

    Packet pkt;
    pkt.type = 0xFF00;
    pkt.tick = tick;

    // Pack tick (4 bytes) + hash (8 bytes) into payload
    pkt.payload.resize(sizeof(uint32_t) + sizeof(uint64_t));
    std::memcpy(pkt.payload.data(), &tick, sizeof(uint32_t));
    std::memcpy(pkt.payload.data() + sizeof(uint32_t), &stateHash, sizeof(uint64_t));
    pkt.size = static_cast<uint16_t>(pkt.payload.size());

    Broadcast(pkt);
}

uint32_t NetContext::LastSaveTick() const {
    return m_lastSaveTick;
}

uint64_t NetContext::LastSaveHash() const {
    return m_lastSaveHash;
}

}
