#pragma once
#include "../ui/EditorPanel.h"
#include "../../engine/sim/ReplayRecorder.h"
#include <string>
#include <vector>
#include <cstdint>

namespace atlas::editor {

enum class MarkerType {
    Bookmark,
    Branch,
    Injection,
    Divergence
};

struct TimelineMarker {
    uint32_t tick = 0;
    std::string label;
    MarkerType type = MarkerType::Bookmark;
};

struct ComparisonResult {
    int64_t firstTick = 0;
    int64_t lastTick = 0;
    int64_t divergeTick = -1;
    double matchPercentage = 100.0;
};

class ReplayTimelinePanel : public EditorPanel {
public:
    const char* Name() const override { return "Replay Timeline"; }
    void Draw() override;

    // Load / query
    void LoadReplay(const std::vector<atlas::sim::ReplayFrame>& frames, uint32_t tickRate);
    void SetCurrentTick(uint32_t tick);
    uint32_t GetCurrentTick() const;
    uint32_t TotalTicks() const;
    uint32_t TickRate() const;
    const atlas::sim::ReplayFrame* CurrentFrame() const;

    // Markers
    void AddMarker(const TimelineMarker& marker);
    void RemoveMarker(uint32_t tick);
    const std::vector<TimelineMarker>& Markers() const;
    std::vector<TimelineMarker> MarkersInRange(uint32_t start, uint32_t end) const;

    // Comparison
    ComparisonResult CompareWith(const std::vector<atlas::sim::ReplayFrame>& other) const;
    bool HasDivergence() const;
    int64_t DivergenceTick() const;

    // Input injection
    void InjectInput(uint32_t tick, const std::vector<uint8_t>& inputData);
    bool HasInjectedInputs() const;

    // Branching
    std::vector<atlas::sim::ReplayFrame> BranchAt(uint32_t tick);

private:
    std::vector<atlas::sim::ReplayFrame> m_frames;
    std::vector<TimelineMarker> m_markers;
    uint32_t m_currentTick = 0;
    uint32_t m_tickRate = 0;
    mutable ComparisonResult m_comparisonResult;
    mutable bool m_hasComparison = false;
};

}
