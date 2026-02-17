#include "VulkanRenderer.h"
#include "../core/Logger.h"

namespace atlas::render {

void VulkanRenderer::SetViewport(int32_t width, int32_t height) {
    m_viewportWidth = width;
    m_viewportHeight = height;
}

void VulkanRenderer::BeginFrame() {
    m_drawCommands.clear();
    m_frameActive = true;
    Logger::Info("[VulkanRenderer] BeginFrame " + std::to_string(m_frameCount));
}

void VulkanRenderer::EndFrame() {
    if (!m_drawCommands.empty()) {
        SubmitCommandBuffer();
    }
    m_frameActive = false;
    ++m_frameCount;
    Logger::Info("[VulkanRenderer] EndFrame — " + std::to_string(m_drawCommands.size()) + " commands recorded");
}

void VulkanRenderer::DrawRect(const ui::UIRect& rect, const ui::UIColor& color) {
    VkDrawCommand cmd;
    cmd.kind = VkDrawCommand::Kind::Rect;
    cmd.rect = rect;
    cmd.color = color;
    m_drawCommands.push_back(cmd);
}

void VulkanRenderer::DrawText(const ui::UIRect& rect, const std::string& text, const ui::UIColor& color) {
    VkDrawCommand cmd;
    cmd.kind = VkDrawCommand::Kind::Text;
    cmd.rect = rect;
    cmd.color = color;
    cmd.text = text;
    m_drawCommands.push_back(cmd);
}

void VulkanRenderer::DrawIcon(const ui::UIRect& rect, uint32_t iconId, const ui::UIColor& tint) {
    VkDrawCommand cmd;
    cmd.kind = VkDrawCommand::Kind::Icon;
    cmd.rect = rect;
    cmd.color = tint;
    cmd.resourceId = iconId;
    m_drawCommands.push_back(cmd);
}

void VulkanRenderer::DrawBorder(const ui::UIRect& rect, int32_t thickness, const ui::UIColor& color) {
    VkDrawCommand cmd;
    cmd.kind = VkDrawCommand::Kind::Border;
    cmd.rect = rect;
    cmd.color = color;
    cmd.thickness = thickness;
    m_drawCommands.push_back(cmd);
}

void VulkanRenderer::DrawImage(const ui::UIRect& rect, uint32_t textureId, const ui::UIColor& tint) {
    VkDrawCommand cmd;
    cmd.kind = VkDrawCommand::Kind::Image;
    cmd.rect = rect;
    cmd.color = tint;
    cmd.resourceId = textureId;
    m_drawCommands.push_back(cmd);
}

const std::vector<VkDrawCommand>& VulkanRenderer::DrawCommands() const {
    return m_drawCommands;
}

size_t VulkanRenderer::DrawCommandCount() const {
    return m_drawCommands.size();
}

bool VulkanRenderer::IsFrameActive() const {
    return m_frameActive;
}

uint32_t VulkanRenderer::FrameCount() const {
    return m_frameCount;
}

void VulkanRenderer::SubmitCommandBuffer() {
    VkGPUCommandBuffer buffer;
    buffer.frameIndex = m_frameCount;
    buffer.commands = m_drawCommands;
    buffer.submitted = true;
    buffer.submitTimestamp = m_submitCounter++;

    if (m_submittedBuffers.size() >= MAX_BUFFERED_FRAMES) {
        m_submittedBuffers.erase(m_submittedBuffers.begin());
    }
    m_submittedBuffers.push_back(std::move(buffer));

    Logger::Info("[VulkanRenderer] SubmitCommandBuffer — frame " +
                 std::to_string(m_frameCount) + ", " +
                 std::to_string(m_drawCommands.size()) + " commands, timestamp " +
                 std::to_string(buffer.submitTimestamp));
}

const VkGPUCommandBuffer& VulkanRenderer::LastSubmittedBuffer() const {
    static const VkGPUCommandBuffer empty{};
    if (m_submittedBuffers.empty()) {
        return empty;
    }
    return m_submittedBuffers.back();
}

uint32_t VulkanRenderer::SubmittedBufferCount() const {
    return static_cast<uint32_t>(m_submitCounter);
}

bool VulkanRenderer::HasPendingCommands() const {
    return !m_drawCommands.empty();
}

} // namespace atlas::render
