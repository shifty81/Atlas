#pragma once
#include "../ui/EditorPanel.h"
#include "../../engine/net/NetContext.h"

namespace atlas::editor {

struct NetInspectorSnapshot {
    std::string modeString;
    uint32_t peerCount = 0;
    float avgRtt = 0.0f;
    uint32_t connectedCount = 0;
};

class NetInspectorPanel : public EditorPanel {
public:
    explicit NetInspectorPanel(net::NetContext& net) : m_net(net) {}

    const char* Name() const override { return "Network"; }
    void Draw() override;

    const NetInspectorSnapshot& Snapshot() const { return m_snapshot; }

private:
    net::NetContext& m_net;
    NetInspectorSnapshot m_snapshot;
};

}
