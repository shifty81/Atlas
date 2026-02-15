#include "ReplayDivergenceInspector.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace atlas::sim {

// ---------------------------------------------------------------------------
// Static helpers
// ---------------------------------------------------------------------------

static DivergenceSeverity ClassifySeverity(const DivergenceReport& r) {
    if (r.divergeTick < 0)
        return DivergenceSeverity::None;
    if (r.totalTicksCompared == 0 || r.firstMatchingTicks == 0)
        return DivergenceSeverity::Critical;
    double ratio = static_cast<double>(r.firstMatchingTicks)
                   / static_cast<double>(r.totalTicksCompared);
    return (ratio < 0.5) ? DivergenceSeverity::Critical
                         : DivergenceSeverity::Warning;
}

// ---------------------------------------------------------------------------
// Compare two StateHasher ladders
// ---------------------------------------------------------------------------

DivergenceReport ReplayDivergenceInspector::Compare(
        const StateHasher& local,
        const StateHasher& remote) {

    const auto& lh = local.History();
    const auto& rh = remote.History();

    DivergenceReport report;
    report.totalTicksCompared = static_cast<uint32_t>(
        std::min(lh.size(), rh.size()));

    for (uint32_t i = 0; i < report.totalTicksCompared; ++i) {
        if (lh[i].hash != rh[i].hash) {
            report.divergeTick      = static_cast<int64_t>(lh[i].tick);
            report.localHash        = lh[i].hash;
            report.remoteHash       = rh[i].hash;
            report.firstMatchingTicks = i;
            report.severity         = ClassifySeverity(report);
            return report;
        }
    }

    // No divergence
    report.firstMatchingTicks = report.totalTicksCompared;
    report.severity           = DivergenceSeverity::None;
    return report;
}

// ---------------------------------------------------------------------------
// Compare two frame streams
// ---------------------------------------------------------------------------

DivergenceReport ReplayDivergenceInspector::CompareFrames(
        const std::vector<ReplayFrame>& a,
        const std::vector<ReplayFrame>& b) {

    DivergenceReport report;
    report.totalTicksCompared = static_cast<uint32_t>(
        std::min(a.size(), b.size()));

    for (uint32_t i = 0; i < report.totalTicksCompared; ++i) {
        if (a[i].stateHash != b[i].stateHash) {
            report.divergeTick      = static_cast<int64_t>(a[i].tick);
            report.localHash        = a[i].stateHash;
            report.remoteHash       = b[i].stateHash;
            report.firstMatchingTicks = i;
            report.severity         = ClassifySeverity(report);
            return report;
        }
    }

    report.firstMatchingTicks = report.totalTicksCompared;
    report.severity           = DivergenceSeverity::None;
    return report;
}

// ---------------------------------------------------------------------------
// Single-frame check
// ---------------------------------------------------------------------------

bool ReplayDivergenceInspector::CheckFrame(
        const ReplayFrame& expected,
        const ReplayFrame& actual) {
    return expected.tick == actual.tick
        && expected.stateHash == actual.stateHash;
}

// ---------------------------------------------------------------------------
// Human-readable analysis
// ---------------------------------------------------------------------------

std::string ReplayDivergenceInspector::AnalyzePattern(
        const DivergenceReport& report) {
    if (report.divergeTick < 0)
        return "No divergence detected. Simulations are in sync.";

    std::ostringstream os;
    os << "Divergence at tick " << report.divergeTick << ". ";
    os << report.firstMatchingTicks << " of "
       << report.totalTicksCompared << " ticks matched before mismatch. ";

    switch (report.severity) {
        case DivergenceSeverity::Critical:
            os << "CRITICAL: early divergence suggests a fundamental desync.";
            break;
        case DivergenceSeverity::Warning:
            os << "WARNING: late divergence may indicate an intermittent issue.";
            break;
        default:
            break;
    }
    return os.str();
}

// ---------------------------------------------------------------------------
// Report formatting
// ---------------------------------------------------------------------------

std::string ReplayDivergenceInspector::FormatReport(
        const DivergenceReport& report) {
    std::ostringstream os;
    os << "=== Divergence Report ===" << "\n";

    if (report.divergeTick < 0) {
        os << "Status    : OK — no divergence\n";
    } else {
        os << "Status    : DIVERGED\n";
        os << "Tick      : " << report.divergeTick << "\n";
        os << "Local     : 0x" << std::hex << std::setw(16)
           << std::setfill('0') << report.localHash << "\n";
        os << "Remote    : 0x" << std::hex << std::setw(16)
           << std::setfill('0') << report.remoteHash << "\n";
        os << std::dec;
    }

    os << "Compared  : " << report.totalTicksCompared << " ticks\n";
    os << "Matched   : " << report.firstMatchingTicks << " ticks\n";

    const char* sevStr = "None";
    if (report.severity == DivergenceSeverity::Warning)  sevStr = "Warning";
    if (report.severity == DivergenceSeverity::Critical) sevStr = "Critical";
    os << "Severity  : " << sevStr << "\n";

    return os.str();
}

// ---------------------------------------------------------------------------
// Severity query
// ---------------------------------------------------------------------------

bool ReplayDivergenceInspector::IsCritical(const DivergenceReport& report) {
    return report.severity == DivergenceSeverity::Critical;
}

// ---------------------------------------------------------------------------
// Instance history
// ---------------------------------------------------------------------------

void ReplayDivergenceInspector::RecordReport(const DivergenceReport& report) {
    m_lastReport = report;
    m_reportsHistory.push_back(report);
    if (m_reportsHistory.size() > m_maxReports) {
        m_reportsHistory.erase(m_reportsHistory.begin());
    }
}

const DivergenceReport& ReplayDivergenceInspector::LastReport() const {
    return m_lastReport;
}

const std::vector<DivergenceReport>&
ReplayDivergenceInspector::ReportHistory() const {
    return m_reportsHistory;
}

void ReplayDivergenceInspector::ClearHistory() {
    m_reportsHistory.clear();
    m_lastReport = DivergenceReport{};
}

}  // namespace atlas::sim
