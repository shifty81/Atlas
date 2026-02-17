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

} // namespace atlas::render
