#include "CIDashboardPanel.h"
#include <algorithm>
#include <sstream>

namespace atlas::editor {

CIDashboardPanel::CIDashboardPanel() = default;

void CIDashboardPanel::Draw() {
    // UI rendering handled by editor backend; data is maintained via the
    // public API (StartPipelineRun, AddCheckResult, CompletePipelineRun).
}

uint64_t CIDashboardPanel::StartPipelineRun(const std::string& commitHash,
                                             const std::string& branch) {
    CIPipelineRun run;
    run.runId = m_nextRunId++;
    run.status = CIPipelineStatus::Running;
    run.commitHash = commitHash;
    run.branch = branch;
    m_runs.push_back(std::move(run));

    if (m_runs.size() > m_maxHistory) {
        m_runs.erase(m_runs.begin(),
                     m_runs.begin() + static_cast<std::ptrdiff_t>(m_runs.size() - m_maxHistory));
    }

    return m_runs.back().runId;
}

void CIDashboardPanel::AddCheckResult(const CICheckResult& result) {
    if (m_runs.empty()) return;
    m_runs.back().checks.push_back(result);
}

void CIDashboardPanel::CompletePipelineRun() {
    if (m_runs.empty()) return;
    CIPipelineRun& run = m_runs.back();

    double total = 0.0;
    for (const auto& c : run.checks) {
        total += c.durationSeconds;
    }
    run.totalDuration = total;

    UpdatePipelineStatus(run);
}

void CIDashboardPanel::UpdatePipelineStatus(CIPipelineRun& run) {
    if (run.checks.empty()) {
        run.status = CIPipelineStatus::Idle;
        return;
    }

    bool anyFailed = false;
    bool anyRunning = false;
    bool anyPassed = false;

    for (const auto& c : run.checks) {
        if (c.status == CICheckStatus::Failed)  anyFailed = true;
        if (c.status == CICheckStatus::Running)  anyRunning = true;
        if (c.status == CICheckStatus::Passed)   anyPassed = true;
    }

    if (anyRunning) {
        run.status = CIPipelineStatus::Running;
    } else if (anyFailed && anyPassed) {
        run.status = CIPipelineStatus::PartialFailure;
    } else if (anyFailed) {
        run.status = CIPipelineStatus::Failed;
    } else if (anyPassed) {
        run.status = CIPipelineStatus::Passed;
    } else {
        run.status = CIPipelineStatus::Idle;
    }
}

CIPipelineStatus CIDashboardPanel::CurrentStatus() const {
    if (m_runs.empty()) return CIPipelineStatus::Idle;
    return m_runs.back().status;
}

const CIPipelineRun* CIDashboardPanel::CurrentRun() const {
    if (m_runs.empty()) return nullptr;
    return &m_runs.back();
}

const CIPipelineRun* CIDashboardPanel::GetRun(uint64_t runId) const {
    for (const auto& run : m_runs) {
        if (run.runId == runId) return &run;
    }
    return nullptr;
}

const std::vector<CIPipelineRun>& CIDashboardPanel::RunHistory() const {
    return m_runs;
}

size_t CIDashboardPanel::RunCount() const {
    return m_runs.size();
}

double CIDashboardPanel::PassRate() const {
    if (m_runs.empty()) return 0.0;
    int passed = 0;
    for (const auto& run : m_runs) {
        if (run.status == CIPipelineStatus::Passed) ++passed;
    }
    return static_cast<double>(passed) / static_cast<double>(m_runs.size());
}

int CIDashboardPanel::TotalChecks() const {
    int total = 0;
    for (const auto& run : m_runs) {
        total += static_cast<int>(run.checks.size());
    }
    return total;
}

int CIDashboardPanel::TotalPassed() const {
    int count = 0;
    for (const auto& run : m_runs) {
        for (const auto& c : run.checks) {
            if (c.status == CICheckStatus::Passed) ++count;
        }
    }
    return count;
}

int CIDashboardPanel::TotalFailed() const {
    int count = 0;
    for (const auto& run : m_runs) {
        for (const auto& c : run.checks) {
            if (c.status == CICheckStatus::Failed) ++count;
        }
    }
    return count;
}

std::vector<CICheckResult> CIDashboardPanel::CurrentChecks() const {
    if (m_runs.empty()) return {};
    return m_runs.back().checks;
}

std::vector<CICheckResult> CIDashboardPanel::CurrentFailedChecks() const {
    if (m_runs.empty()) return {};
    std::vector<CICheckResult> failed;
    for (const auto& c : m_runs.back().checks) {
        if (c.status == CICheckStatus::Failed) failed.push_back(c);
    }
    return failed;
}

void CIDashboardPanel::ClearHistory() {
    m_runs.clear();
}

void CIDashboardPanel::SetMaxHistory(size_t max) {
    m_maxHistory = max;
    if (m_runs.size() > m_maxHistory) {
        m_runs.erase(m_runs.begin(),
                     m_runs.begin() + static_cast<std::ptrdiff_t>(m_runs.size() - m_maxHistory));
    }
}

size_t CIDashboardPanel::GetMaxHistory() const {
    return m_maxHistory;
}

std::string CIDashboardPanel::Summary() const {
    if (m_runs.empty()) return "CI: no runs";
    const CIPipelineRun& run = m_runs.back();
    int passed = 0;
    for (const auto& c : run.checks) {
        if (c.status == CICheckStatus::Passed) ++passed;
    }
    std::ostringstream os;
    os << "CI: " << passed << "/" << run.checks.size()
       << " passed (" << m_runs.size() << " runs total)";
    return os.str();
}

}
