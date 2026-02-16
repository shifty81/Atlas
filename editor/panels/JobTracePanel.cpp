#include "JobTracePanel.h"
#include <cstdio>

namespace atlas::editor {

void JobTracePanel::SetTracer(const sim::JobTracer* tracer) {
    m_tracer = tracer;
}

void JobTracePanel::SetReferenceTracer(const sim::JobTracer* reference) {
    m_reference = reference;
}

void JobTracePanel::Refresh() {
    m_summaries.clear();
    m_firstMismatch = -1;

    if (!m_tracer) return;

    const auto& history = m_tracer->History();

    for (const auto& trace : history) {
        JobTraceSummary summary;
        summary.tick = trace.tick;
        summary.systemCount = static_cast<uint32_t>(trace.entries.size());
        summary.orderHash = trace.orderHash;
        summary.orderMatches = true;

        // Compare against reference if available
        if (m_reference) {
            const auto* refTrace = m_reference->TraceAtTick(trace.tick);
            if (refTrace) {
                summary.orderMatches = (trace.orderHash == refTrace->orderHash);
            }
        }

        if (!summary.orderMatches && m_firstMismatch < 0) {
            m_firstMismatch = static_cast<int64_t>(trace.tick);
        }

        m_summaries.push_back(summary);
    }
}

const std::vector<JobTraceSummary>& JobTracePanel::Summaries() const {
    return m_summaries;
}

bool JobTracePanel::HasOrderMismatch() const {
    return m_firstMismatch >= 0;
}

int64_t JobTracePanel::FirstMismatchTick() const {
    return m_firstMismatch;
}

std::string JobTracePanel::Summary() const {
    if (!m_tracer) return "No tracer attached";
    if (m_summaries.empty()) return "No traces recorded";
    if (!HasOrderMismatch()) return "Execution order consistent";

    char buf[128];
    std::snprintf(buf, sizeof(buf),
        "Order mismatch at tick %lld",
        (long long)m_firstMismatch);
    return std::string(buf);
}

std::vector<sim::JobTraceEntry> JobTracePanel::EntriesAtTick(uint64_t tick) const {
    if (!m_tracer) return {};
    const auto* trace = m_tracer->TraceAtTick(tick);
    if (!trace) return {};
    return trace->entries;
}

}  // namespace atlas::editor
