# Atlas Engine Lockdown Checklist

## Purpose

Atlas is considered **feature-locked** only when every item on this checklist is verified.
This checklist serves as the gate between "in development" and "stable platform".

Feature additions after lockdown require explicit contract review.
Determinism bugs after lockdown are treated as P0 severity.

---

## Core

- [ ] Core Contract ratified and committed (ATLAS_CORE_CONTRACT.md)
- [x] Formal world state model enforced (simulation/derived/metadata categories)
- [x] Time model locked (simulation/world/presentation domains)
- [ ] Determinism enforcement layer active (compile-time + runtime + CI)
- [x] Deterministic allocator implemented and tested
- [x] Canonical world state definition documented

## Simulation

- [ ] Fixed tick rate immutable (60 Hz default, configurable at build time only)
- [ ] All systems registered and execution order locked
- [ ] No hidden state in any simulation system
- [ ] System mutation ownership enforced
- [ ] Mid-tick mutation prevention active

## Replay & Networking

- [x] Replay binary format versioned and frozen
- [ ] Network packet schema frozen
- [ ] Golden replays pass CI on all target platforms
- [x] Hash ladder verification operational
- [x] Replay divergence detection and reporting functional
- [ ] Save/load determinism verified across replay boundaries
- [ ] Rollback/resimulation produces identical hashes

## Assets

- [ ] Asset schemas locked and versioned
- [ ] Hash-based asset validation enforced
- [ ] Editor diff → commit flow complete
- [ ] Server-safe asset validation operational
- [ ] Asset immutability enforced at API level

## GUI System

- [ ] GUI layout solver produces identical output on all platforms
- [ ] GUI input replay functional
- [ ] GUI DSL compiler operational
- [ ] All main editor panels present and functional

## Tooling

- [ ] Truth UI (minimal editor) functional
- [ ] Replay divergence inspectable in editor
- [ ] One-command reproduction of any determinism bug
- [ ] State hash visualizer operational
- [ ] Job execution trace viewer available

## Formal Verification

- [ ] ecs.tla passes TLC model checker
- [ ] replay.tla passes TLC model checker
- [ ] layout.tla passes TLC model checker
- [ ] CI runs TLC on every spec-touching PR

## CI & Policy

- [ ] Golden replay CI gate active
- [ ] Contract bot active on all PRs
- [ ] determinism.json change detection gate active
- [ ] Feature additions require contract review
- [ ] Determinism violations block merge
- [ ] Cross-platform replay verification in CI

---

## Lockdown Declaration

When all items above are checked, Atlas may be declared **locked**.

Post-lockdown rules:
- All changes require contract review
- Determinism bugs are P0 (highest priority)
- Replay format changes require version bump and migration
- New modules must declare layer, dependencies, and mutation scope
