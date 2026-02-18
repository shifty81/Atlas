#pragma once
#include <cstdint>

namespace atlas::ui {

/// Maximum widget ID to check when iterating.  This is a reasonable upper
/// bound for editor UI.  All managers share this constant to stay in sync.
static constexpr uint32_t kMaxWidgetId = 1000;

} // namespace atlas::ui
