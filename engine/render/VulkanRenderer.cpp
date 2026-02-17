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

    uint64_t ts = buffer.submitTimestamp;
    size_t cmdCount = buffer.commands.size();

    if (m_submittedBuffers.size() >= MAX_BUFFERED_FRAMES) {
        m_submittedBuffers.erase(m_submittedBuffers.begin());
    }
    m_submittedBuffers.push_back(std::move(buffer));

    Logger::Info("[VulkanRenderer] SubmitCommandBuffer — frame " +
                 std::to_string(m_frameCount) + ", " +
                 std::to_string(cmdCount) + " commands, timestamp " +
                 std::to_string(ts));
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

// --- Render pass management ---

uint32_t VulkanRenderer::CreateRenderPass(const VkRenderPassDesc& desc) {
    VkRenderPassDesc pass = desc;
    uint32_t id = m_nextPassId++;
    m_renderPasses.push_back(pass);
    Logger::Info("[VulkanRenderer] CreateRenderPass '" + desc.name + "' id=" + std::to_string(id));
    return id;
}

void VulkanRenderer::BeginRenderPass(uint32_t passId) {
    if (passId == 0 || passId >= m_nextPassId) return;
    m_activeRenderPass = passId;
    m_renderPassActive = true;
    Logger::Info("[VulkanRenderer] BeginRenderPass id=" + std::to_string(passId));
}

void VulkanRenderer::EndRenderPass() {
    if (!m_renderPassActive) return;
    Logger::Info("[VulkanRenderer] EndRenderPass id=" + std::to_string(m_activeRenderPass));
    m_activeRenderPass = 0;
    m_renderPassActive = false;
}

bool VulkanRenderer::IsRenderPassActive() const {
    return m_renderPassActive;
}

const VkRenderPassDesc* VulkanRenderer::GetRenderPass(uint32_t id) const {
    if (id == 0 || id > m_renderPasses.size()) return nullptr;
    return &m_renderPasses[id - 1];
}

uint32_t VulkanRenderer::RenderPassCount() const {
    return static_cast<uint32_t>(m_renderPasses.size());
}

// --- Pipeline state management ---

uint32_t VulkanRenderer::CreatePipelineState(const VkPipelineStateDesc& desc) {
    VkPipelineStateDesc state = desc;
    state.id = m_nextPipelineId++;
    m_pipelineStates.push_back(state);
    Logger::Info("[VulkanRenderer] CreatePipelineState id=" + std::to_string(state.id) +
                 " vs=" + desc.vertexShader + " fs=" + desc.fragmentShader);
    return state.id;
}

void VulkanRenderer::BindPipeline(uint32_t pipelineId) {
    if (pipelineId == 0 || pipelineId >= m_nextPipelineId) return;
    m_boundPipeline = pipelineId;
    Logger::Info("[VulkanRenderer] BindPipeline id=" + std::to_string(pipelineId));
}

uint32_t VulkanRenderer::BoundPipelineId() const {
    return m_boundPipeline;
}

const VkPipelineStateDesc* VulkanRenderer::GetPipelineState(uint32_t id) const {
    if (id == 0 || id > m_pipelineStates.size()) return nullptr;
    return &m_pipelineStates[id - 1];
}

uint32_t VulkanRenderer::PipelineStateCount() const {
    return static_cast<uint32_t>(m_pipelineStates.size());
}

// --- GPU resource management ---

uint32_t VulkanRenderer::CreateBuffer(VkGPUResource::Type type, size_t sizeBytes) {
    VkGPUResource res;
    res.type = type;
    res.id = m_nextBufferId++;
    res.sizeBytes = sizeBytes;
    res.mapped = false;
    m_buffers.push_back(res);
    Logger::Info("[VulkanRenderer] CreateBuffer id=" + std::to_string(res.id) +
                 " size=" + std::to_string(sizeBytes));
    return res.id;
}

bool VulkanRenderer::DestroyBuffer(uint32_t bufferId) {
    for (auto it = m_buffers.begin(); it != m_buffers.end(); ++it) {
        if (it->id == bufferId) {
            Logger::Info("[VulkanRenderer] DestroyBuffer id=" + std::to_string(bufferId));
            m_buffers.erase(it);
            return true;
        }
    }
    return false;
}

const VkGPUResource* VulkanRenderer::GetBuffer(uint32_t id) const {
    for (const auto& buf : m_buffers) {
        if (buf.id == id) return &buf;
    }
    return nullptr;
}

uint32_t VulkanRenderer::BufferCount() const {
    return static_cast<uint32_t>(m_buffers.size());
}

bool VulkanRenderer::MapBuffer(uint32_t bufferId) {
    for (auto& buf : m_buffers) {
        if (buf.id == bufferId) {
            if (buf.mapped) return false;
            buf.mapped = true;
            Logger::Info("[VulkanRenderer] MapBuffer id=" + std::to_string(bufferId));
            return true;
        }
    }
    return false;
}

bool VulkanRenderer::UnmapBuffer(uint32_t bufferId) {
    for (auto& buf : m_buffers) {
        if (buf.id == bufferId) {
            if (!buf.mapped) return false;
            buf.mapped = false;
            Logger::Info("[VulkanRenderer] UnmapBuffer id=" + std::to_string(bufferId));
            return true;
        }
    }
    return false;
}

} // namespace atlas::render
