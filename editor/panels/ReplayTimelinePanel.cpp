#include "ReplayTimelinePanel.h"
#include <algorithm>

namespace atlas::editor {

void ReplayTimelinePanel::Draw() {
    // A full UI backend would render the timeline scrubber, markers, and
    // comparison overlay here.  All state is maintained by the public API.
}

void ReplayTimelinePanel::LoadReplay(const std::vector<atlas::sim::ReplayFrame>& frames,
                                     uint32_t tickRate) {
    m_frames = frames;
    m_tickRate = tickRate;
    m_currentTick = 0;
    m_markers.clear();
    m_hasComparison = false;
    m_comparisonResult = ComparisonResult{};
}

void ReplayTimelinePanel::SetCurrentTick(uint32_t tick) {
    if (m_frames.empty()) {
        m_currentTick = 0;
        return;
    }
    m_currentTick = std::min(tick, m_frames.back().tick);
}

uint32_t ReplayTimelinePanel::GetCurrentTick() const {
    return m_currentTick;
}

uint32_t ReplayTimelinePanel::TotalTicks() const {
    if (m_frames.empty()) return 0;
    return m_frames.back().tick + 1;
}

uint32_t ReplayTimelinePanel::TickRate() const {
    return m_tickRate;
}

const atlas::sim::ReplayFrame* ReplayTimelinePanel::CurrentFrame() const {
    if (m_frames.empty()) return nullptr;
    atlas::sim::ReplayFrame key;
    key.tick = m_currentTick;
    auto it = std::lower_bound(m_frames.begin(), m_frames.end(), key,
        [](const atlas::sim::ReplayFrame& a, const atlas::sim::ReplayFrame& b) {
            return a.tick < b.tick;
        });
    if (it != m_frames.end() && it->tick == m_currentTick) return &(*it);
    return nullptr;
}

// --- Markers ---

void ReplayTimelinePanel::AddMarker(const TimelineMarker& marker) {
    m_markers.push_back(marker);
}

void ReplayTimelinePanel::RemoveMarker(uint32_t tick) {
    m_markers.erase(
        std::remove_if(m_markers.begin(), m_markers.end(),
                       [tick](const TimelineMarker& m) { return m.tick == tick; }),
        m_markers.end());
}

const std::vector<TimelineMarker>& ReplayTimelinePanel::Markers() const {
    return m_markers;
}

std::vector<TimelineMarker> ReplayTimelinePanel::MarkersInRange(uint32_t start,
                                                                 uint32_t end) const {
    std::vector<TimelineMarker> result;
    for (const auto& m : m_markers) {
        if (m.tick >= start && m.tick <= end) {
            result.push_back(m);
        }
    }
    return result;
}

// --- Comparison ---

ComparisonResult ReplayTimelinePanel::CompareWith(
    const std::vector<atlas::sim::ReplayFrame>& other) const {
    ComparisonResult result;

    if (m_frames.empty() && other.empty()) {
        m_comparisonResult = result;
        m_hasComparison = true;
        return result;
    }

    result.firstTick = 0;
    size_t minLen = std::min(m_frames.size(), other.size());
    size_t maxLen = std::max(m_frames.size(), other.size());
    result.lastTick = static_cast<int64_t>(maxLen > 0 ? maxLen - 1 : 0);

    size_t matchCount = 0;
    result.divergeTick = -1;

    for (size_t i = 0; i < minLen; ++i) {
        if (m_frames[i].stateHash == other[i].stateHash &&
            m_frames[i].inputData == other[i].inputData) {
            ++matchCount;
        } else if (result.divergeTick < 0) {
            result.divergeTick = static_cast<int64_t>(m_frames[i].tick);
        }
    }

    // Frames beyond the shorter replay count as divergent
    if (minLen < maxLen && result.divergeTick < 0) {
        result.divergeTick = static_cast<int64_t>(minLen);
    }

    result.matchPercentage = maxLen > 0
        ? (static_cast<double>(matchCount) / static_cast<double>(maxLen)) * 100.0
        : 100.0;

    m_comparisonResult = result;
    m_hasComparison = true;
    return result;
}

bool ReplayTimelinePanel::HasDivergence() const {
    return m_hasComparison && m_comparisonResult.divergeTick >= 0;
}

int64_t ReplayTimelinePanel::DivergenceTick() const {
    if (!m_hasComparison) return -1;
    return m_comparisonResult.divergeTick;
}

// --- Input injection ---

void ReplayTimelinePanel::InjectInput(uint32_t tick,
                                      const std::vector<uint8_t>& inputData) {
    for (auto& f : m_frames) {
        if (f.tick == tick) {
            f.inputData = inputData;
            return;
        }
    }
    // If no frame exists at this tick, insert in sorted order
    atlas::sim::ReplayFrame newFrame;
    newFrame.tick = tick;
    newFrame.inputData = inputData;
    auto it = std::lower_bound(m_frames.begin(), m_frames.end(), newFrame,
        [](const atlas::sim::ReplayFrame& a, const atlas::sim::ReplayFrame& b) {
            return a.tick < b.tick;
        });
    m_frames.insert(it, newFrame);
}

bool ReplayTimelinePanel::HasInjectedInputs() const {
    // Any frame with non-empty inputData that was modified counts;
    // for simplicity we track whether any frame has data at all.
    for (const auto& f : m_frames) {
        if (!f.inputData.empty()) return true;
    }
    return false;
}

// --- Branching ---

std::vector<atlas::sim::ReplayFrame> ReplayTimelinePanel::BranchAt(uint32_t tick) {
    std::vector<atlas::sim::ReplayFrame> branch;
    for (const auto& f : m_frames) {
        if (f.tick <= tick) {
            branch.push_back(f);
        }
    }
    return branch;
}

}
