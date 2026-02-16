#pragma once
#include "FlowGraphIR.h"
#include <string>

namespace atlas::flow {

/// Stub for future Flow Graph → C++ code generation.
/// Release builds will compile flow graphs to native C++ for maximum performance.
class FlowGraphCodegen {
public:
    /// Generate C++ source from a compiled flow graph IR.
    /// Returns empty string if codegen is not yet implemented.
    static std::string Generate(const FlowGraphIR& ir) {
        (void)ir;
        return ""; // Placeholder — codegen pipeline not yet implemented
    }
};

} // namespace atlas::flow
