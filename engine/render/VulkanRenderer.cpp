#include "VulkanRenderer.h"
#include "../core/Logger.h"
#include <cstring>
#include <algorithm>

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

// --- Descriptor set management ---

uint32_t VulkanRenderer::CreateDescriptorSetLayout(const VkDescriptorSetLayoutDesc& desc) {
    VkDescriptorSetLayoutDesc layout = desc;
    layout.id = m_nextDescriptorSetId++;
    m_descriptorSetLayouts.push_back(layout);
    Logger::Info("[VulkanRenderer] CreateDescriptorSetLayout '" + desc.name + "' id=" + std::to_string(layout.id));
    return layout.id;
}

void VulkanRenderer::BindDescriptorSet(uint32_t layoutId) {
    if (layoutId == 0 || layoutId >= m_nextDescriptorSetId) return;
    m_boundDescriptorSet = layoutId;
    Logger::Info("[VulkanRenderer] BindDescriptorSet id=" + std::to_string(layoutId));
}

uint32_t VulkanRenderer::BoundDescriptorSetId() const {
    return m_boundDescriptorSet;
}

const VkDescriptorSetLayoutDesc* VulkanRenderer::GetDescriptorSetLayout(uint32_t id) const {
    if (id == 0 || id > m_descriptorSetLayouts.size()) return nullptr;
    return &m_descriptorSetLayouts[id - 1];
}

uint32_t VulkanRenderer::DescriptorSetLayoutCount() const {
    return static_cast<uint32_t>(m_descriptorSetLayouts.size());
}

// --- Texture management ---

uint32_t VulkanRenderer::CreateTexture(const VkTextureDesc& desc) {
    VkTextureDesc tex = desc;
    tex.id = m_nextTextureId++;
    m_textures.push_back(tex);
    Logger::Info("[VulkanRenderer] CreateTexture '" + desc.name + "' id=" + std::to_string(tex.id) +
                 " " + std::to_string(desc.width) + "x" + std::to_string(desc.height));
    return tex.id;
}

bool VulkanRenderer::DestroyTexture(uint32_t textureId) {
    for (auto it = m_textures.begin(); it != m_textures.end(); ++it) {
        if (it->id == textureId) {
            Logger::Info("[VulkanRenderer] DestroyTexture id=" + std::to_string(textureId));
            m_textures.erase(it);
            return true;
        }
    }
    return false;
}

const VkTextureDesc* VulkanRenderer::GetTexture(uint32_t id) const {
    for (const auto& tex : m_textures) {
        if (tex.id == id) return &tex;
    }
    return nullptr;
}

uint32_t VulkanRenderer::TextureCount() const {
    return static_cast<uint32_t>(m_textures.size());
}

// --- Sampler management ---

uint32_t VulkanRenderer::CreateSampler(const VkSamplerDesc& desc) {
    VkSamplerDesc sampler = desc;
    sampler.id = m_nextSamplerId++;
    m_samplers.push_back(sampler);
    Logger::Info("[VulkanRenderer] CreateSampler '" + desc.name + "' id=" + std::to_string(sampler.id));
    return sampler.id;
}

bool VulkanRenderer::DestroySampler(uint32_t samplerId) {
    for (auto it = m_samplers.begin(); it != m_samplers.end(); ++it) {
        if (it->id == samplerId) {
            Logger::Info("[VulkanRenderer] DestroySampler id=" + std::to_string(samplerId));
            m_samplers.erase(it);
            return true;
        }
    }
    return false;
}

const VkSamplerDesc* VulkanRenderer::GetSampler(uint32_t id) const {
    for (const auto& s : m_samplers) {
        if (s.id == id) return &s;
    }
    return nullptr;
}

uint32_t VulkanRenderer::SamplerCount() const {
    return static_cast<uint32_t>(m_samplers.size());
}

// --- Push constant management ---

const std::vector<uint8_t> VulkanRenderer::s_emptyPushData;

uint32_t VulkanRenderer::RegisterPushConstantRange(const VkPushConstantRange& range) {
    VkPushConstantRange r = range;
    r.id = m_nextPushConstantId++;
    m_pushConstantRanges.push_back(r);
    m_pushConstantData.emplace_back(); // empty data slot
    Logger::Info("[VulkanRenderer] RegisterPushConstantRange '" + r.name +
                 "' offset=" + std::to_string(r.offset) +
                 " size=" + std::to_string(r.size) +
                 " id=" + std::to_string(r.id));
    return r.id;
}

bool VulkanRenderer::PushConstants(uint32_t rangeId, const void* data, uint32_t sizeBytes) {
    if (rangeId == 0 || rangeId >= m_nextPushConstantId) return false;
    size_t idx = rangeId - 1;
    if (idx >= m_pushConstantRanges.size()) return false;
    const auto& range = m_pushConstantRanges[idx];
    if (sizeBytes > range.size) return false;
    auto& buf = m_pushConstantData[idx];
    buf.resize(sizeBytes);
    std::memcpy(buf.data(), data, sizeBytes);
    Logger::Info("[VulkanRenderer] PushConstants rangeId=" + std::to_string(rangeId) +
                 " bytes=" + std::to_string(sizeBytes));
    return true;
}

const VkPushConstantRange* VulkanRenderer::GetPushConstantRange(uint32_t id) const {
    if (id == 0 || id >= m_nextPushConstantId) return nullptr;
    return &m_pushConstantRanges[id - 1];
}

uint32_t VulkanRenderer::PushConstantRangeCount() const {
    return static_cast<uint32_t>(m_pushConstantRanges.size());
}

const std::vector<uint8_t>& VulkanRenderer::PushConstantData(uint32_t rangeId) const {
    if (rangeId == 0 || rangeId >= m_nextPushConstantId) return s_emptyPushData;
    size_t idx = rangeId - 1;
    if (idx >= m_pushConstantData.size()) return s_emptyPushData;
    return m_pushConstantData[idx];
}

// --- Shader uniform management ---

uint32_t VulkanRenderer::BindUniform(const VkShaderUniform& uniform) {
    VkShaderUniform u = uniform;
    u.id = m_nextUniformId++;
    m_uniforms.push_back(u);
    Logger::Info("[VulkanRenderer] BindUniform '" + u.name +
                 "' set=" + std::to_string(u.set) +
                 " binding=" + std::to_string(u.binding) +
                 " id=" + std::to_string(u.id));
    return u.id;
}

bool VulkanRenderer::UpdateUniform(uint32_t uniformId, const void* data, uint32_t sizeBytes) {
    for (auto& u : m_uniforms) {
        if (u.id == uniformId) {
            // sizeBytes == 0 means the uniform was created without an initial size
            // (flexible); otherwise enforce the declared size as an upper bound.
            if (u.sizeBytes != 0 && sizeBytes > u.sizeBytes) return false;
            u.data.resize(sizeBytes);
            std::memcpy(u.data.data(), data, sizeBytes);
            u.sizeBytes = sizeBytes;
            return true;
        }
    }
    return false;
}

const VkShaderUniform* VulkanRenderer::GetUniform(uint32_t id) const {
    for (const auto& u : m_uniforms) {
        if (u.id == id) return &u;
    }
    return nullptr;
}

const VkShaderUniform* VulkanRenderer::GetUniformByName(const std::string& name) const {
    for (const auto& u : m_uniforms) {
        if (u.name == name) return &u;
    }
    return nullptr;
}

uint32_t VulkanRenderer::UniformCount() const {
    return static_cast<uint32_t>(m_uniforms.size());
}

void VulkanRenderer::ClearUniforms() {
    m_uniforms.clear();
    m_nextUniformId = 1;
}

// --- Fence management ---

uint32_t VulkanRenderer::CreateFence(const std::string& name, bool signaled) {
    VkFenceDesc fence;
    fence.name = name;
    fence.signaled = signaled;
    fence.id = m_nextFenceId++;
    m_fences.push_back(fence);
    Logger::Info("[VulkanRenderer] CreateFence '" + name + "' id=" + std::to_string(fence.id));
    return fence.id;
}

bool VulkanRenderer::DestroyFence(uint32_t fenceId) {
    for (auto it = m_fences.begin(); it != m_fences.end(); ++it) {
        if (it->id == fenceId) {
            Logger::Info("[VulkanRenderer] DestroyFence id=" + std::to_string(fenceId));
            m_fences.erase(it);
            return true;
        }
    }
    return false;
}

bool VulkanRenderer::WaitFence(uint32_t fenceId) {
    for (auto& f : m_fences) {
        if (f.id == fenceId) {
            f.signaled = true;
            Logger::Info("[VulkanRenderer] WaitFence id=" + std::to_string(fenceId));
            return true;
        }
    }
    return false;
}

bool VulkanRenderer::ResetFence(uint32_t fenceId) {
    for (auto& f : m_fences) {
        if (f.id == fenceId) {
            f.signaled = false;
            Logger::Info("[VulkanRenderer] ResetFence id=" + std::to_string(fenceId));
            return true;
        }
    }
    return false;
}

bool VulkanRenderer::IsFenceSignaled(uint32_t fenceId) const {
    for (const auto& f : m_fences) {
        if (f.id == fenceId) return f.signaled;
    }
    return false;
}

const VkFenceDesc* VulkanRenderer::GetFence(uint32_t id) const {
    for (const auto& f : m_fences) {
        if (f.id == id) return &f;
    }
    return nullptr;
}

uint32_t VulkanRenderer::FenceCount() const {
    return static_cast<uint32_t>(m_fences.size());
}

// --- Semaphore management ---

uint32_t VulkanRenderer::CreateSemaphore(const std::string& name) {
    VkSemaphoreDesc sem;
    sem.name = name;
    sem.signaled = false;
    sem.id = m_nextSemaphoreId++;
    m_semaphores.push_back(sem);
    Logger::Info("[VulkanRenderer] CreateSemaphore '" + name + "' id=" + std::to_string(sem.id));
    return sem.id;
}

bool VulkanRenderer::DestroySemaphore(uint32_t semaphoreId) {
    for (auto it = m_semaphores.begin(); it != m_semaphores.end(); ++it) {
        if (it->id == semaphoreId) {
            Logger::Info("[VulkanRenderer] DestroySemaphore id=" + std::to_string(semaphoreId));
            m_semaphores.erase(it);
            return true;
        }
    }
    return false;
}

bool VulkanRenderer::SignalSemaphore(uint32_t semaphoreId) {
    for (auto& s : m_semaphores) {
        if (s.id == semaphoreId) {
            s.signaled = true;
            Logger::Info("[VulkanRenderer] SignalSemaphore id=" + std::to_string(semaphoreId));
            return true;
        }
    }
    return false;
}

bool VulkanRenderer::WaitSemaphore(uint32_t semaphoreId) {
    for (auto& s : m_semaphores) {
        if (s.id == semaphoreId) {
            if (!s.signaled) return false;
            s.signaled = false;
            Logger::Info("[VulkanRenderer] WaitSemaphore id=" + std::to_string(semaphoreId));
            return true;
        }
    }
    return false;
}

const VkSemaphoreDesc* VulkanRenderer::GetSemaphore(uint32_t id) const {
    for (const auto& s : m_semaphores) {
        if (s.id == id) return &s;
    }
    return nullptr;
}

uint32_t VulkanRenderer::SemaphoreCount() const {
    return static_cast<uint32_t>(m_semaphores.size());
}

// --- Memory pool management ---

uint32_t VulkanRenderer::CreateMemoryPool(const std::string& name, size_t totalSize) {
    VkMemoryPool pool;
    pool.name = name;
    pool.totalSize = totalSize;
    pool.usedSize = 0;
    pool.allocationCount = 0;
    pool.id = m_nextPoolId++;
    m_memoryPools.push_back(pool);
    Logger::Info("[VulkanRenderer] CreateMemoryPool '" + name + "' id=" + std::to_string(pool.id) +
                 " size=" + std::to_string(totalSize));
    return pool.id;
}

bool VulkanRenderer::DestroyMemoryPool(uint32_t poolId) {
    for (auto it = m_memoryPools.begin(); it != m_memoryPools.end(); ++it) {
        if (it->id == poolId) {
            Logger::Info("[VulkanRenderer] DestroyMemoryPool id=" + std::to_string(poolId));
            // Remove all allocations from this pool
            m_allocations.erase(
                std::remove_if(m_allocations.begin(), m_allocations.end(),
                    [poolId](const VkMemoryAllocation& a) { return a.poolId == poolId; }),
                m_allocations.end());
            m_memoryPools.erase(it);
            return true;
        }
    }
    return false;
}

uint32_t VulkanRenderer::AllocateFromPool(uint32_t poolId, size_t size) {
    // Linear bump allocator: freed blocks are not reused until pool is destroyed.
    // This is intentional for frame-scoped allocations where fragmentation is
    // avoided by resetting the pool each frame.
    for (auto& pool : m_memoryPools) {
        if (pool.id == poolId) {
            if (pool.usedSize + size > pool.totalSize) {
                Logger::Info("[VulkanRenderer] AllocateFromPool failed: not enough space in pool " +
                             std::to_string(poolId));
                return 0;
            }
            VkMemoryAllocation alloc;
            alloc.poolId = poolId;
            alloc.offset = pool.usedSize;
            alloc.size = size;
            alloc.id = m_nextAllocationId++;
            pool.usedSize += size;
            pool.allocationCount++;
            m_allocations.push_back(alloc);
            Logger::Info("[VulkanRenderer] AllocateFromPool pool=" + std::to_string(poolId) +
                         " id=" + std::to_string(alloc.id) + " size=" + std::to_string(size));
            return alloc.id;
        }
    }
    return 0;
}

bool VulkanRenderer::FreeAllocation(uint32_t allocationId) {
    for (auto it = m_allocations.begin(); it != m_allocations.end(); ++it) {
        if (it->id == allocationId) {
            // Return size to pool
            for (auto& pool : m_memoryPools) {
                if (pool.id == it->poolId) {
                    pool.usedSize -= it->size;
                    pool.allocationCount--;
                    break;
                }
            }
            Logger::Info("[VulkanRenderer] FreeAllocation id=" + std::to_string(allocationId));
            m_allocations.erase(it);
            return true;
        }
    }
    return false;
}

const VkMemoryPool* VulkanRenderer::GetMemoryPool(uint32_t id) const {
    for (const auto& p : m_memoryPools) {
        if (p.id == id) return &p;
    }
    return nullptr;
}

const VkMemoryAllocation* VulkanRenderer::GetAllocation(uint32_t id) const {
    for (const auto& a : m_allocations) {
        if (a.id == id) return &a;
    }
    return nullptr;
}

uint32_t VulkanRenderer::MemoryPoolCount() const {
    return static_cast<uint32_t>(m_memoryPools.size());
}

size_t VulkanRenderer::PoolUsedSize(uint32_t poolId) const {
    for (const auto& p : m_memoryPools) {
        if (p.id == poolId) return p.usedSize;
    }
    return 0;
}

size_t VulkanRenderer::PoolFreeSize(uint32_t poolId) const {
    for (const auto& p : m_memoryPools) {
        if (p.id == poolId) return p.totalSize - p.usedSize;
    }
    return 0;
}

} // namespace atlas::render
