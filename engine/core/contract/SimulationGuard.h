#pragma once
// ============================================================
// Atlas Simulation Guard
// ============================================================
//
// Enforces that world state mutations only occur during an
// active simulation tick. Any mutation outside tick boundaries
// is a contract violation.
//
// Usage:
//   ATLAS_SIM_TICK_BEGIN();
//   // ... simulation systems run ...
//   ATLAS_SIM_TICK_END();
//
//   // In mutation code:
//   ATLAS_SIM_MUTATION_GUARD();  // traps if not inside a tick
//
// See: docs/ATLAS_CORE_CONTRACT.md
//      docs/ATLAS_DETERMINISM_ENFORCEMENT.md

#include <atomic>

namespace atlas {

/// Global flag indicating whether a simulation tick is active.
/// Only the tick scheduler should set this.
inline std::atomic<bool> g_simulation_tick_active{false};

}  // namespace atlas

#define ATLAS_SIM_TICK_BEGIN() \
    ::atlas::g_simulation_tick_active.store(true, std::memory_order_release)

#define ATLAS_SIM_TICK_END() \
    ::atlas::g_simulation_tick_active.store(false, std::memory_order_release)

#if defined(NDEBUG)
  #define ATLAS_SIM_MUTATION_GUARD() ((void)0)
#else
  #define ATLAS_SIM_MUTATION_GUARD() \
      do { \
          if (!::atlas::g_simulation_tick_active.load(std::memory_order_acquire)) { \
              __builtin_trap(); \
          } \
      } while (0)
#endif
