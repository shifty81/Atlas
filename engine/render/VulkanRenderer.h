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

    static constexpr uint32_t MAX_BUFFERED_FRAMES = 3;

private:
    int32_t m_viewportWidth = 1280;
    int32_t m_viewportHeight = 720;
    std::vector<VkDrawCommand> m_drawCommands;
    bool m_frameActive = false;
    uint32_t m_frameCount = 0;
    std::vector<VkGPUCommandBuffer> m_submittedBuffers;
    uint64_t m_submitCounter = 0;
};

} // namespace atlas::render
