# Output-targeted workspace activation and display logging

- **Date:** 2026-11-11
- **CLI Tool:** Codex CLI 0.167.0
- **Model:** GPT-6.1 Sol (`gpt-6.1-sol`)
- **Base:** `v0.56.2-fred.3`
- **Transcript:** Retained privately by the author.
- **Scope:** Source development only; no package installation or activation.

## User request

> For [redacted: private plan reference], make side-monitor workspace repair preserve focus, then finish native logging before the physical Dell trial.

> Proceed

## Implementation

Adds `hl.dsp.workspace.select({workspace="5", monitor="DP-1"})` for bounded normal workspace IDs 1–160. Resolves an exact output, creates or assigns the named workspace, then activates it without borrowing another output's input focus or moving the pointer. Refuses to move the focused output's active workspace away. Side activation does not release mouse buttons. A compact workspace-selected record reports the observed input-output identity.

Adds a common diagnostic sink for compositor warnings/errors and the Aquamarine callback. Records contain wall and CLOCK_MONOTONIC emission nanoseconds, component, severity and PID; normal backend events use a small transition allowlist. Records stay below 4096 bytes. Stdout carries journal priority prefixes; display.log flushes each record. Existing verbose Hyprland logging settings and its original file remain independent. Sink failure is reported to stderr. File opens reject symlinks and nonregular files.

Retire the local action when upstream offers equivalent bounded, output-targeted activation with the same no-focus/no-pointer behavior. Retire the logging patch when upstream provides equivalent emission clocks, filtering and promptly available journal/file records.

## Verification and limits

All four changed translation units pass syntax checks against the matching release headers. The exact emission sink passes isolated compiled tests for filtering, bounds, clocks, newline handling, file visibility and symlink refusal. A proof linked to the rebuilt Aquamarine library verifies its real logger callback plus the common sink, with matching journal/file payloads and journal priorities 3,3,3,6,4. Complete Hyprland package linking, candidate service routing, idle overhead and physical input continuity are still unproven.

The journal envelope is not an exact per-line emission clock: the synthetic stream observation included a receive-minus-emission delta of approximately −2 microseconds. Payload CLOCK_MONOTONIC samples preserve emission ordering independently of stream batching and wall-clock corrections. File flushing establishes visibility to readers, not fsync/power-loss durability.
