#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace atlas::sim {

struct ReplayFrame {
    uint32_t tick = 0;
    std::vector<uint8_t> inputData;
};

struct ReplayHeader {
    uint32_t magic = 0x52504C59;  // "RPLY"
    uint32_t version = 1;
    uint32_t tickRate = 30;
    uint32_t frameCount = 0;
    uint32_t seed = 0;
};

enum class ReplayState {
    Idle,
    Recording,
    Playing,
};

class ReplayRecorder {
public:
    // Recording
    void StartRecording(uint32_t tickRate, uint32_t seed = 0);
    void RecordFrame(uint32_t tick, const std::vector<uint8_t>& inputData);
    void StopRecording();

    // Playback
    bool LoadReplay(const std::string& path);
    bool SaveReplay(const std::string& path) const;
    const ReplayFrame* FrameAtTick(uint32_t tick) const;

    // Query
    ReplayState State() const;
    const ReplayHeader& Header() const;
    const std::vector<ReplayFrame>& Frames() const;
    size_t FrameCount() const;
    uint32_t DurationTicks() const;

    // Reset
    void Clear();

private:
    ReplayState m_state = ReplayState::Idle;
    ReplayHeader m_header;
    std::vector<ReplayFrame> m_frames;
};

}
