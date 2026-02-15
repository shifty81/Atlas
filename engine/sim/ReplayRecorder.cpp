#include "ReplayRecorder.h"
#include <fstream>

namespace atlas::sim {

void ReplayRecorder::StartRecording(uint32_t tickRate, uint32_t seed) {
    m_frames.clear();
    m_header = ReplayHeader{};
    m_header.tickRate = tickRate;
    m_header.seed = seed;
    m_state = ReplayState::Recording;
}

void ReplayRecorder::RecordFrame(uint32_t tick, const std::vector<uint8_t>& inputData) {
    if (m_state != ReplayState::Recording) return;
    ReplayFrame frame;
    frame.tick = tick;
    frame.inputData = inputData;
    m_frames.push_back(std::move(frame));
    m_header.frameCount = static_cast<uint32_t>(m_frames.size());
}

void ReplayRecorder::StopRecording() {
    m_state = ReplayState::Idle;
}

bool ReplayRecorder::LoadReplay(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return false;

    ReplayHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(header));
    if (header.magic != 0x52504C59) return false;

    m_header = header;
    m_frames.clear();
    m_frames.reserve(header.frameCount);

    for (uint32_t i = 0; i < header.frameCount; ++i) {
        ReplayFrame frame;
        file.read(reinterpret_cast<char*>(&frame.tick), sizeof(frame.tick));
        uint32_t dataSize = 0;
        file.read(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));
        frame.inputData.resize(dataSize);
        if (dataSize > 0) {
            file.read(reinterpret_cast<char*>(frame.inputData.data()), dataSize);
        }
        m_frames.push_back(std::move(frame));
    }

    m_state = ReplayState::Playing;
    return true;
}

bool ReplayRecorder::SaveReplay(const std::string& path) const {
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) return false;

    file.write(reinterpret_cast<const char*>(&m_header), sizeof(m_header));

    for (const auto& frame : m_frames) {
        file.write(reinterpret_cast<const char*>(&frame.tick), sizeof(frame.tick));
        uint32_t dataSize = static_cast<uint32_t>(frame.inputData.size());
        file.write(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));
        if (dataSize > 0) {
            file.write(reinterpret_cast<const char*>(frame.inputData.data()), dataSize);
        }
    }

    return true;
}

const ReplayFrame* ReplayRecorder::FrameAtTick(uint32_t tick) const {
    for (const auto& frame : m_frames) {
        if (frame.tick == tick) return &frame;
    }
    return nullptr;
}

ReplayState ReplayRecorder::State() const {
    return m_state;
}

const ReplayHeader& ReplayRecorder::Header() const {
    return m_header;
}

const std::vector<ReplayFrame>& ReplayRecorder::Frames() const {
    return m_frames;
}

size_t ReplayRecorder::FrameCount() const {
    return m_frames.size();
}

uint32_t ReplayRecorder::DurationTicks() const {
    if (m_frames.empty()) return 0;
    return m_frames.back().tick;
}

void ReplayRecorder::Clear() {
    m_state = ReplayState::Idle;
    m_header = ReplayHeader{};
    m_frames.clear();
}

}
