#pragma once
#include "../ui/UIRenderer.h"
#include <cstdint>
#include <string>
#include <vector>

#ifdef DrawText
#undef DrawText
#endif

namespace atlas::render {

struct VkDrawCommand {
    enum class Kind : uint8_t {
        Rect,
        Text,
        Icon,
        Border,
        Image
    };

    Kind kind = Kind::Rect;
    ui::UIRect rect{};
    ui::UIColor color{};
    int32_t thickness = 0;
    uint32_t resourceId = 0;
    std::string text;
};

struct VkGPUCommandBuffer {
    uint32_t frameIndex = 0;
    std::vector<VkDrawCommand> commands;
    bool submitted = false;
    uint64_t submitTimestamp = 0;
};

struct VkRenderPassDesc {
    std::string name;
    uint32_t colorAttachmentCount = 1;
    bool hasDepth = false;
    bool clear = true;
};

struct VkPipelineStateDesc {
    std::string vertexShader;
    std::string fragmentShader;
    bool depthTest = true;
    bool depthWrite = true;
    bool blending = false;
    uint32_t id = 0;
};

struct VkGPUResource {
    enum class Type : uint8_t { VertexBuffer, IndexBuffer, UniformBuffer };
    Type type = Type::VertexBuffer;
    uint32_t id = 0;
    size_t sizeBytes = 0;
    bool mapped = false;
};

struct VkDescriptorSetLayoutDesc {
    std::string name;
    uint32_t bindingCount = 0;
    uint32_t id = 0;
};

struct VkTextureDesc {
    std::string name;
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t mipLevels = 1;
    uint32_t id = 0;
};

struct VkSamplerDesc {
    std::string name;
    bool linearFilter = true;
    bool clampToEdge = true;
    uint32_t id = 0;
};

/// A push-constant block pushed to a shader stage.
struct VkPushConstantRange {
    std::string name;
    uint32_t offset = 0;
    uint32_t size = 0;   ///< Size in bytes (max 128 bytes per Vulkan spec)
    uint32_t id = 0;
};

/// A scalar/vector/matrix uniform value bound to a shader.
struct VkShaderUniform {
    std::string name;
    uint32_t binding = 0;
    uint32_t set = 0;
    uint32_t sizeBytes = 0;
    std::vector<uint8_t> data;
    uint32_t id = 0;
};

class VulkanRenderer : public ui::UIRenderer {
public:
    void BeginFrame() override;
    void EndFrame() override;

    void DrawRect(const ui::UIRect& rect, const ui::UIColor& color) override;
    void DrawText(const ui::UIRect& rect, const std::string& text, const ui::UIColor& color) override;
    void DrawIcon(const ui::UIRect& rect, uint32_t iconId, const ui::UIColor& tint) override;
    void DrawBorder(const ui::UIRect& rect, int32_t thickness, const ui::UIColor& color) override;
    void DrawImage(const ui::UIRect& rect, uint32_t textureId, const ui::UIColor& tint) override;

    void SetViewport(int32_t width, int32_t height);

    const std::vector<VkDrawCommand>& DrawCommands() const;
    size_t DrawCommandCount() const;
    bool IsFrameActive() const;
    uint32_t FrameCount() const;

    void SubmitCommandBuffer();
    const VkGPUCommandBuffer& LastSubmittedBuffer() const;
    uint32_t SubmittedBufferCount() const;
    bool HasPendingCommands() const;

    // Render pass management
    uint32_t CreateRenderPass(const VkRenderPassDesc& desc);
    void BeginRenderPass(uint32_t passId);
    void EndRenderPass();
    bool IsRenderPassActive() const;
    const VkRenderPassDesc* GetRenderPass(uint32_t id) const;
    uint32_t RenderPassCount() const;

    // Pipeline state management
    uint32_t CreatePipelineState(const VkPipelineStateDesc& desc);
    void BindPipeline(uint32_t pipelineId);
    uint32_t BoundPipelineId() const;
    const VkPipelineStateDesc* GetPipelineState(uint32_t id) const;
    uint32_t PipelineStateCount() const;

    // GPU resource management
    uint32_t CreateBuffer(VkGPUResource::Type type, size_t sizeBytes);
    bool DestroyBuffer(uint32_t bufferId);
    const VkGPUResource* GetBuffer(uint32_t id) const;
    uint32_t BufferCount() const;
    bool MapBuffer(uint32_t bufferId);
    bool UnmapBuffer(uint32_t bufferId);

    // Descriptor set management
    uint32_t CreateDescriptorSetLayout(const VkDescriptorSetLayoutDesc& desc);
    void BindDescriptorSet(uint32_t layoutId);
    uint32_t BoundDescriptorSetId() const;
    const VkDescriptorSetLayoutDesc* GetDescriptorSetLayout(uint32_t id) const;
    uint32_t DescriptorSetLayoutCount() const;

    // Texture management
    uint32_t CreateTexture(const VkTextureDesc& desc);
    bool DestroyTexture(uint32_t textureId);
    const VkTextureDesc* GetTexture(uint32_t id) const;
    uint32_t TextureCount() const;

    // Sampler management
    uint32_t CreateSampler(const VkSamplerDesc& desc);
    bool DestroySampler(uint32_t samplerId);
    const VkSamplerDesc* GetSampler(uint32_t id) const;
    uint32_t SamplerCount() const;

    // Push constant management
    uint32_t RegisterPushConstantRange(const VkPushConstantRange& range);
    bool PushConstants(uint32_t rangeId, const void* data, uint32_t sizeBytes);
    const VkPushConstantRange* GetPushConstantRange(uint32_t id) const;
    uint32_t PushConstantRangeCount() const;
    const std::vector<uint8_t>& PushConstantData(uint32_t rangeId) const;

    // Shader uniform management
    uint32_t BindUniform(const VkShaderUniform& uniform);
    bool UpdateUniform(uint32_t uniformId, const void* data, uint32_t sizeBytes);
    const VkShaderUniform* GetUniform(uint32_t id) const;
    const VkShaderUniform* GetUniformByName(const std::string& name) const;
    uint32_t UniformCount() const;
    void ClearUniforms();

    static constexpr uint32_t MAX_BUFFERED_FRAMES = 3;

private:
    int32_t m_viewportWidth = 1280;
    int32_t m_viewportHeight = 720;
    std::vector<VkDrawCommand> m_drawCommands;
    bool m_frameActive = false;
    uint32_t m_frameCount = 0;
    std::vector<VkGPUCommandBuffer> m_submittedBuffers;
    uint64_t m_submitCounter = 0;

    std::vector<VkRenderPassDesc> m_renderPasses;
    uint32_t m_activeRenderPass = 0;
    bool m_renderPassActive = false;

    std::vector<VkPipelineStateDesc> m_pipelineStates;
    uint32_t m_boundPipeline = 0;

    std::vector<VkGPUResource> m_buffers;
    uint32_t m_nextBufferId = 1;
    uint32_t m_nextPassId = 1;
    uint32_t m_nextPipelineId = 1;

    std::vector<VkDescriptorSetLayoutDesc> m_descriptorSetLayouts;
    uint32_t m_boundDescriptorSet = 0;
    uint32_t m_nextDescriptorSetId = 1;

    std::vector<VkTextureDesc> m_textures;
    uint32_t m_nextTextureId = 1;

    std::vector<VkSamplerDesc> m_samplers;
    uint32_t m_nextSamplerId = 1;

    std::vector<VkPushConstantRange> m_pushConstantRanges;
    std::vector<std::vector<uint8_t>>  m_pushConstantData;
    uint32_t m_nextPushConstantId = 1;

    std::vector<VkShaderUniform> m_uniforms;
    uint32_t m_nextUniformId = 1;

    static const std::vector<uint8_t> s_emptyPushData;
};

} // namespace atlas::render
