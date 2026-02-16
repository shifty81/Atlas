#pragma once
#include <cstdint>

namespace atlas::render {

enum class RenderAPI : uint8_t {
    OpenGL,
    Vulkan
};

} // namespace atlas::render
