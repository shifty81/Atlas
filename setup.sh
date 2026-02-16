#!/usr/bin/env bash
#
# Atlas Engine — First-Run Setup
#
# Checks prerequisites, configures, builds, and tests the engine.
# Safe to run multiple times (idempotent).
#
# Usage:
#   ./setup.sh                  # Full setup with defaults
#   ./setup.sh --skip-tests     # Skip running tests after build
#   ./setup.sh --build-type Debug
#

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SOURCE_DIR="$SCRIPT_DIR"
BUILD_DIR="$SOURCE_DIR/build"
BUILD_TYPE="Release"
SKIP_TESTS=false

# --- Colors (disabled if not a terminal) ---
if [ -t 1 ]; then
    RED='\033[0;31m'
    GREEN='\033[0;32m'
    YELLOW='\033[1;33m'
    CYAN='\033[0;36m'
    BOLD='\033[1m'
    RESET='\033[0m'
else
    RED='' GREEN='' YELLOW='' CYAN='' BOLD='' RESET=''
fi

info()  { echo -e "${CYAN}[INFO]${RESET}  $*"; }
ok()    { echo -e "${GREEN}[OK]${RESET}    $*"; }
warn()  { echo -e "${YELLOW}[WARN]${RESET}  $*"; }
error() { echo -e "${RED}[ERROR]${RESET} $*" >&2; }

# --- Parse arguments ---
while [[ $# -gt 0 ]]; do
    case "$1" in
        --skip-tests)
            SKIP_TESTS=true; shift ;;
        --build-type)
            BUILD_TYPE="$2"; shift 2 ;;
        -h|--help)
            echo "Usage: ./setup.sh [--skip-tests] [--build-type TYPE]"
            echo ""
            echo "Options:"
            echo "  --skip-tests         Skip running tests after build"
            echo "  --build-type TYPE    Build type: Debug, Release, Development (default: Release)"
            echo "  -h, --help           Show this help message"
            exit 0 ;;
        *)
            error "Unknown option: $1"; exit 1 ;;
    esac
done

# --- Setup log ---
LOG_DIR="$SOURCE_DIR/logs"
mkdir -p "$LOG_DIR"
SETUP_LOG="$LOG_DIR/setup.log"

# Redirect all output to both console and setup log
exec > >(tee -a "$SETUP_LOG") 2>&1

# Write a header to the setup log
echo ""
echo "=== Atlas Setup Log ==="
echo "Started: $(date '+%Y-%m-%d %H:%M:%S')"
echo ""

echo ""
echo -e "${BOLD}╔══════════════════════════════════════╗${RESET}"
echo -e "${BOLD}║      Atlas Engine — First-Run Setup   ║${RESET}"
echo -e "${BOLD}╚══════════════════════════════════════╝${RESET}"
echo ""

# ──────────────────────────────────────
# Step 1: Check prerequisites
# ──────────────────────────────────────
info "Step 1/5: Checking prerequisites..."

HAS_ERRORS=false

# CMake
if command -v cmake &>/dev/null; then
    cmake_version="$(cmake --version | head -n1)"
    ok "  cmake found: $cmake_version"
else
    error "  cmake not found. Please install CMake 3.22 or later."
    HAS_ERRORS=true
fi

# C++ compiler
CXX_COMPILER=""
if command -v g++ &>/dev/null; then
    CXX_COMPILER="g++"
    cxx_version="$(g++ --version | head -n1)"
    ok "  C++ compiler found: $cxx_version"
elif command -v clang++ &>/dev/null; then
    CXX_COMPILER="clang++"
    cxx_version="$(clang++ --version | head -n1)"
    ok "  C++ compiler found: $cxx_version"
else
    error "  No C++ compiler found. Please install g++ or clang++."
    HAS_ERRORS=true
fi

# Make / Ninja
if command -v ninja &>/dev/null; then
    ok "  Build tool: ninja"
elif command -v make &>/dev/null; then
    ok "  Build tool: make"
else
    error "  No build tool found. Please install make or ninja."
    HAS_ERRORS=true
fi

# Git (optional but recommended)
if command -v git &>/dev/null; then
    ok "  git found"
else
    warn "  git not found (optional, but recommended)"
fi

if [ "$HAS_ERRORS" = true ]; then
    echo ""
    error "Missing prerequisites — please install the tools listed above and re-run."
    exit 1
fi

echo ""
ok "All prerequisites satisfied."
echo ""

# ──────────────────────────────────────
# Step 2: Create build directory
# ──────────────────────────────────────
info "Step 2/5: Preparing build directory..."

mkdir -p "$BUILD_DIR"
ok "Build directory ready: $BUILD_DIR"
echo ""

# ──────────────────────────────────────
# Step 3: Configure with CMake
# ──────────────────────────────────────
info "Step 3/5: Running CMake configure..."

# Map build type
CMAKE_BUILD_TYPE="$BUILD_TYPE"
case "$BUILD_TYPE" in
    Debug|debug)       CMAKE_BUILD_TYPE="Debug" ;;
    Release|release)   CMAKE_BUILD_TYPE="Release" ;;
    Development|development|Dev|dev) CMAKE_BUILD_TYPE="RelWithDebInfo" ;;
    *) error "Unknown build type: $BUILD_TYPE"; exit 1 ;;
esac

cmake -S "$SOURCE_DIR" -B "$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE="$CMAKE_BUILD_TYPE" \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    2>&1 | while IFS= read -r line; do echo "  $line"; done

ok "CMake configured ($CMAKE_BUILD_TYPE)"
echo ""

# ──────────────────────────────────────
# Step 4: Build the engine
# ──────────────────────────────────────
info "Step 4/5: Building Atlas Engine..."

# Detect parallel jobs
if command -v nproc &>/dev/null; then
    JOBS=$(nproc)
elif command -v sysctl &>/dev/null; then
    JOBS=$(sysctl -n hw.ncpu 2>/dev/null || echo 4)
else
    JOBS=4
fi

cmake --build "$BUILD_DIR" -j "$JOBS" \
    2>&1 | while IFS= read -r line; do echo "  $line"; done

ok "Build complete"
echo ""

# ──────────────────────────────────────
# Step 5: Run tests
# ──────────────────────────────────────
if [ "$SKIP_TESTS" = false ]; then
    info "Step 5/5: Running tests..."
    TEST_BIN="$BUILD_DIR/tests/AtlasTests"
    if [ -f "$TEST_BIN" ]; then
        set +e
        test_output=$("$TEST_BIN" 2>&1)
        test_exit=$?
        set -e
        echo "$test_output" | while IFS= read -r line; do echo "  $line"; done
        if [ "$test_exit" -ne 0 ]; then
            error "Some tests failed (exit code $test_exit)"
            warn "You can re-run tests later with: ./build.sh --test tests"
        else
            ok "All tests passed"
        fi
    else
        warn "Test binary not found — skipping tests"
        warn "Build tests explicitly with: ./build.sh tests"
    fi
else
    info "Step 5/5: Skipping tests (--skip-tests)"
fi

# ──────────────────────────────────────
# Done
# ──────────────────────────────────────
echo ""
echo -e "${BOLD}═══════════════════════════════════════${RESET}"
echo -e "${GREEN}${BOLD} Atlas Engine setup complete!${RESET}"
echo -e "${BOLD}═══════════════════════════════════════${RESET}"
echo ""
info "Quick start:"
info "  ./run.sh editor                   # Launch the editor"
info "  ./run.sh server                   # Launch headless server"
info "  ./run.sh client                   # Launch the client"
info "  ./run.sh runtime <project.atlas>  # Launch runtime with a project"
info "  ./build.sh --run editor           # Build and run the editor"
echo ""
info "For more options, run: ./build.sh --help"
echo ""
echo "=== Setup Finished: $(date '+%Y-%m-%d %H:%M:%S') ==="
info "Setup log saved to: $SETUP_LOG"
echo ""
