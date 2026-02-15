#include "ProfilerPanel.h"
#include <algorithm>
#include <numeric>

namespace atlas::editor {

void ProfilerPanel::Draw() {
    // Stub: would render frame time graph, system metric bars, controls
}

void ProfilerPanel::RecordFrame(const FrameTiming& timing) {
    if (m_paused) return;
    m_history.push_back(timing);
    if (m_history.size() > m_maxHistory) {
        m_history.erase(m_history.begin());
    }
    m_currentMetrics.clear();
}

void ProfilerPanel::RecordSystemMetric(const SystemMetric& metric) {
    if (m_paused) return;
    m_currentMetrics.push_back(metric);
}

const std::vector<FrameTiming>& ProfilerPanel::History() const {
    return m_history;
}

const std::vector<SystemMetric>& ProfilerPanel::CurrentMetrics() const {
    return m_currentMetrics;
}

double ProfilerPanel::AverageFrameTime() const {
    if (m_history.empty()) return 0.0;
    double total = 0.0;
    for (const auto& f : m_history) {
        total += f.frameDurationMs;
    }
    return total / static_cast<double>(m_history.size());
}

double ProfilerPanel::PeakFrameTime() const {
    if (m_history.empty()) return 0.0;
    double peak = 0.0;
    for (const auto& f : m_history) {
        if (f.frameDurationMs > peak) peak = f.frameDurationMs;
    }
    return peak;
}

uint64_t ProfilerPanel::FrameCount() const {
    return m_history.size();
}

void ProfilerPanel::SetMaxHistory(size_t maxFrames) {
    m_maxHistory = maxFrames;
    while (m_history.size() > m_maxHistory) {
        m_history.erase(m_history.begin());
    }
}

size_t ProfilerPanel::MaxHistory() const {
    return m_maxHistory;
}

void ProfilerPanel::ClearHistory() {
    m_history.clear();
    m_currentMetrics.clear();
}

void ProfilerPanel::SetPaused(bool paused) {
    m_paused = paused;
}

bool ProfilerPanel::IsPaused() const {
    return m_paused;
}

}
