#!/usr/bin/env python3
"""
Atlas Contract Scanner

Scans simulation source files for forbidden API usage that
violates the Atlas Core Contract.

Usage:
    python tools/contract_scan.py [--path ENGINE_DIR]

Returns exit code 1 if violations are found.

See: docs/ATLAS_CORE_CONTRACT.md
     docs/ATLAS_DETERMINISM_ENFORCEMENT.md
"""

import argparse
import pathlib
import sys

# APIs forbidden in simulation code
FORBIDDEN_PATTERNS = [
    ("std::chrono", "Non-deterministic time source"),
    ("time(", "Wall-clock time access"),
    ("rand(", "Non-deterministic randomness"),
    ("<random>", "OS-dependent random header"),
    ("srand(", "Non-deterministic seed"),
    ("clock(", "Wall-clock time access"),
    ("gettimeofday", "Wall-clock time access"),
    ("clock_gettime", "Wall-clock time access"),
]

# Directories to scan (relative to engine root)
# These are simulation-critical directories only.
# Utility code (e.g. core/Logger) is excluded because it
# legitimately uses wall-clock time for logging.
SIMULATION_DIRS = [
    "ecs",
    "sim",
    "physics",
    "gameplay",
    "ai",
]

# Directories to explicitly skip (contain references to
# forbidden APIs in comments/error messages, not actual usage)
SKIP_DIRS = {
    "contract",
}

# File extensions to scan
SOURCE_EXTENSIONS = {".cpp", ".h", ".hpp", ".cxx"}


def scan_file(filepath: pathlib.Path) -> list[str]:
    """Scan a single file for contract violations."""
    violations = []
    try:
        text = filepath.read_text(errors="ignore")
    except OSError:
        return violations

    for pattern, reason in FORBIDDEN_PATTERNS:
        if pattern in text:
            violations.append(
                f"{filepath}: uses forbidden API `{pattern}` — {reason}"
            )
    return violations


def main():
    parser = argparse.ArgumentParser(description="Atlas Contract Scanner")
    parser.add_argument(
        "--path",
        default="engine",
        help="Root directory to scan (default: engine)",
    )
    args = parser.parse_args()

    root = pathlib.Path(args.path)
    if not root.exists():
        print(f"Error: directory {root} not found", file=sys.stderr)
        sys.exit(2)

    all_violations = []

    for sim_dir in SIMULATION_DIRS:
        scan_path = root / sim_dir
        if not scan_path.exists():
            continue
        for filepath in scan_path.rglob("*"):
            if filepath.suffix in SOURCE_EXTENSIONS:
                # Skip directories that reference forbidden APIs in
                # comments or error messages (e.g. contract headers)
                if any(skip in filepath.parts for skip in SKIP_DIRS):
                    continue
                all_violations.extend(scan_file(filepath))

    if all_violations:
        print("❌ Atlas Core Contract Violations Detected:\n")
        for v in all_violations:
            print(f"  • {v}")
        print(f"\nTotal: {len(all_violations)} violation(s)")
        print("See: docs/ATLAS_CORE_CONTRACT.md")
        sys.exit(1)
    else:
        print("✅ No contract violations detected.")
        sys.exit(0)


if __name__ == "__main__":
    main()
