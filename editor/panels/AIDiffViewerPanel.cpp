#include "AIDiffViewerPanel.h"

namespace atlas::editor {

void AIDiffViewerPanel::Draw() {
    // UI rendering handled by backend.
}

void AIDiffViewerPanel::LoadDiffSet(const DiffSet& diff) {
    m_currentDiff = diff;
    m_hasDiff = true;
}

const DiffSet* AIDiffViewerPanel::CurrentDiff() const {
    if (!m_hasDiff) return nullptr;
    return &m_currentDiff;
}

bool AIDiffViewerPanel::HasDiff() const {
    return m_hasDiff;
}

void AIDiffViewerPanel::AcceptHunk(size_t fileIndex, size_t hunkIndex) {
    if (fileIndex < m_currentDiff.files.size() &&
        hunkIndex < m_currentDiff.files[fileIndex].hunks.size()) {
        m_currentDiff.files[fileIndex].hunks[hunkIndex].accepted = true;
        m_currentDiff.files[fileIndex].hunks[hunkIndex].rejected = false;
    }
}

void AIDiffViewerPanel::RejectHunk(size_t fileIndex, size_t hunkIndex) {
    if (fileIndex < m_currentDiff.files.size() &&
        hunkIndex < m_currentDiff.files[fileIndex].hunks.size()) {
        m_currentDiff.files[fileIndex].hunks[hunkIndex].rejected = true;
        m_currentDiff.files[fileIndex].hunks[hunkIndex].accepted = false;
    }
}

bool AIDiffViewerPanel::IsHunkAccepted(size_t fileIndex, size_t hunkIndex) const {
    if (fileIndex < m_currentDiff.files.size() &&
        hunkIndex < m_currentDiff.files[fileIndex].hunks.size()) {
        return m_currentDiff.files[fileIndex].hunks[hunkIndex].accepted;
    }
    return false;
}

bool AIDiffViewerPanel::IsHunkRejected(size_t fileIndex, size_t hunkIndex) const {
    if (fileIndex < m_currentDiff.files.size() &&
        hunkIndex < m_currentDiff.files[fileIndex].hunks.size()) {
        return m_currentDiff.files[fileIndex].hunks[hunkIndex].rejected;
    }
    return false;
}

void AIDiffViewerPanel::AcceptAll() {
    for (auto& file : m_currentDiff.files) {
        for (auto& hunk : file.hunks) {
            hunk.accepted = true;
            hunk.rejected = false;
        }
    }
}

void AIDiffViewerPanel::RejectAll() {
    for (auto& file : m_currentDiff.files) {
        for (auto& hunk : file.hunks) {
            hunk.rejected = true;
            hunk.accepted = false;
        }
    }
}

bool AIDiffViewerPanel::ApplyAccepted() {
    if (!m_hasDiff) return false;
    m_currentDiff.applied = true;
    return true;
}

size_t AIDiffViewerPanel::AcceptedHunkCount() const {
    size_t count = 0;
    for (const auto& file : m_currentDiff.files) {
        for (const auto& hunk : file.hunks) {
            if (hunk.accepted) ++count;
        }
    }
    return count;
}

size_t AIDiffViewerPanel::TotalHunkCount() const {
    size_t count = 0;
    for (const auto& file : m_currentDiff.files) {
        count += file.hunks.size();
    }
    return count;
}

void AIDiffViewerPanel::AddToHistory(const DiffSet& diff) {
    m_history.push_back(diff);
}

const std::vector<DiffSet>& AIDiffViewerPanel::History() const {
    return m_history;
}

void AIDiffViewerPanel::Clear() {
    m_currentDiff = DiffSet{};
    m_hasDiff = false;
    m_history.clear();
}

} // namespace atlas::editor
