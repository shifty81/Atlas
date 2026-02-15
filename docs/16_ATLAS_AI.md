# AtlasAI — Intelligent Authoring Assistant

## Overview

AtlasAI is the expanded AI-assisted authoring system for the Atlas Editor.
It builds on the AI aggregator described in `08_AI_EDITOR_ASSIST.md` and adds
web aggregation, context-aware assistance, and full editor integration.

Core principles:

- AI is a tooling aid — never runtime logic
- Context-aware, permission-scoped assistant
- Operates on structured data (assets, graphs, schemas) — never raw magic
- Editor-only: disabled in CI, never in runtime
- If AI is disabled completely, the project still builds and replays identically

## Architecture

```
Editor Context
   ↓
Context Snapshot (read-only)
   ↓
Prompt Builder
   ↓
AI Backend (local or remote)
   ↓
Structured Response (JSON / DSL / AST)
   ↓
Preview + Diff
   ↓
User Approval
   ↓
Deterministic Apply
```

## AtlasAI Core (Orchestrator Model)

Atlas's own AI layer is the authority. External models (ChatGPT, web LLMs) are
pluggable advisors that feed into the orchestrator, not replacements for it.

Three layers, in priority order:

1. **Offline templates** — deterministic, no network required
2. **Web-indexed documentation and knowledge** — local knowledge base built from crawled docs
3. **External LLM integration** — optional, remote or local models

## Context Awareness

Structured context packets are sent to the AI backend, never the whole project.

Context includes:

- Editor mode and active panel
- Selected assets and their schemas
- Open graph context (nodes, edges, subgraph)
- Engine version and feature flags
- User permission level

This keeps responses relevant, cheap, and predictable.

## AI Intents

- **Explain** — describe assets, graphs, systems
- **Generate** — create GUI DSL, flow graphs, asset schemas, AtlasScript
- **Lint** — check graphs for structural issues
- **Debug** — diagnose replay divergence
- **Refactor** — AI-assisted flow graph refactoring
- **Tutorial** — generate interactive tutorials

## Allowed Output Types

- GUI DSL ✅
- Flow Graph JSON ✅
- Asset schemas ✅
- AtlasScript ✅
- Explanations ✅
- Diffs / patches ✅
- C++ code ⚠️ (suggestion only, never auto-applied)

## Safety and Determinism Rules

- AI output is not part of the runtime
- Generated assets are hashed like any other asset
- Replay files never depend on AI presence
- Every output follows: preview → diff → approve
- No mutations without explicit user approval
- All actions are permission-gated

## Permission Scoping

| Permission | Can Generate Assets | Can Suggest Code | Can Explain |
|------------|---------------------|------------------|-------------|
| ViewOnly   | ✖                   | ✖                | ✔           |
| QA         | ✖                   | ✖                | ✔           |
| Developer  | ✔                   | ✔                | ✔           |
| CI         | ✖                   | ✖                | ✖           |
| Admin      | ✔                   | ✔                | ✔           |

## Editor Panels

- **Atlas Assistant panel** — dockable, toggled with `Ctrl+Shift+A`
- **Context menu** — right-click "Ask Atlas about this..." on any asset or node
- **Assistant Diff Viewer** — preview generated changes before applying
- **Prompt history** — recent prompts and suggestions

## Backend Options

- **Remote** — OpenAI, Azure, other LLM APIs
- **Local** — local LLM exposed via API
- **Disabled** — offline or CI mode, no AI calls

All backends are abstracted behind the `IAssistantBackend` interface.

## "Add Content to Project" Flow

1. User prompt: "Add a basic inventory system"
2. AI generates: assets, flow graphs, optional UI DSL
3. Editor shows: asset tree preview, graph diff, schema validation
4. User clicks Apply
5. Assets added via the normal editor pipeline
6. No shortcuts, no magic

## Web Aggregation

- Own web crawler/indexer for documentation and knowledge sources
- Builds a local knowledge base to supplement offline templates
- Privacy-respecting: no project data is sent without explicit consent
- Indexed content is cached and versioned locally

## See Also

- `08_AI_EDITOR_ASSIST.md` — AI aggregator and validation layer
- `15_FLOW_GRAPH.md` — flow graph system targeted by AI generation
- `12_GUI_SYSTEM.md` — GUI DSL that AI can produce
