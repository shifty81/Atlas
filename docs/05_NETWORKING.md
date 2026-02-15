# Atlas Engine — Networking

## Implementation Status

| Feature | Status | Notes |
|---------|--------|-------|
| NetContext API | ✅ Implemented | Mode selection, peer management, packet send/receive |
| Dedicated server loop | ✅ Implemented | Headless mode with tick processing |
| P2P support | ✅ Implemented | Host/peer roles with RTT tracking |
| Loopback mode | ✅ Implemented | Local testing without network |
| Lockstep sync | ⬜ Stub | Methods declared, ECS serialization not implemented |
| Rollback/replay | ⬜ Stub | Methods declared, ECS restore not implemented |
| Replication rules | ⬜ Not started | No implementation |

## Unified Networking Model

Atlas supports multiple networking modes through a single abstraction:

| Mode           | Description                    |
|----------------|--------------------------------|
| Standalone     | No networking                  |
| Client         | Client connecting to server    |
| Server         | Dedicated authoritative server |
| P2P_Host       | Peer-to-peer (host)            |
| P2P_Peer       | Peer-to-peer (peer)            |

## NetContext API

```cpp
enum class NetMode {
    Standalone,
    Client,
    Server,
    P2P_Host,
    P2P_Peer
};

struct NetPeer {
    uint32_t id;
    float rtt;
    bool connected;
};

class NetContext {
public:
    void Init(NetMode mode);
    void Shutdown();
    void Poll();
    void Send(uint32_t peerID, const Packet& pkt);
    void Broadcast(const Packet& pkt);
    void Flush();

    NetMode Mode() const;
    const std::vector<NetPeer>& Peers() const;
    bool IsAuthority() const;
};
```

## Replication Rules

### Authoritative State
- ECS component state
- Simulation ticks
- Graph events

### Client-Only State
- Camera
- UI
- Prediction

### Data Flow

| Data     | Direction            |
|----------|----------------------|
| Input    | Client → Authority   |
| State    | Authority → Clients  |
| Events   | Authority → All      |
| Assets   | Editor → Disk → Runtime |

## Lockstep + Rollback (Planned)

> **Note:** The lockstep and rollback methods exist in `NetContext` but contain
> stub logic. ECS state serialization and restoration need to be implemented
> before these features are functional.

Design goals:

- Fixed tick rate (e.g., 30 Hz)
- Inputs buffered by tick
- Graph VM deterministic
- ECS state snapshot per tick

### Rollback (Planned)

```cpp
// Target API — methods exist but internals are stubs
void OnLateInput(InputFrame f) {
    if (f.tick < currentTick) {
        RollbackTo(f.tick);   // Stub: needs ECS state restore
        ReplayFrom(f.tick);   // Stub: needs ECS state replay
    }
}
```

## Replication Rules (Planned)

> **Note:** Replication rules are not yet implemented. The following describes
> the intended design.

### Authoritative State
