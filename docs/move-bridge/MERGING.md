# Maintainer integration and rollback

## Scope

Changes are isolated from the local test console and unrelated repository work. No ELF, SELF, personal config, saved pairing state, games, firmware, logs, screenshots or private SDK cache is included.

| File | Change |
| --- | --- |
| src/porpoise_move_bridge.hpp | Validated nonblocking loopback client; packet lifetime/sequence rules; MVR1 feedback sender |
| src/porpoise_pad.cpp | Per-player Move/Nav/second-hand sockets, takeover/fallback, menu coexistence, calibration/recenter and Nunchuk input |
| src/porpoise_pad.hpp | Active-device queries, calibration and attachment-toggle API |
| src/porpoise_core.cpp | First-player Nav PS attachment toggle applied through existing Wii configuration |
| src/ui_app_controls.cpp | Move/Nav graphics, Move-specific setup/recenter prompts |
| tools/build-porpoise.sh | Explicit PORPOISE_MOVE_BRIDGE=1 opt-in; normal build unchanged |
| tests/move_bridge/ | Wire/client and actual pad-backend regressions; default-disabled compile |
| docs/move-bridge/, docs/move-studio/ | Maintainer/AI handoff, shared protocol, attribution and limitations |

## Suggested review order

1. Review the standalone wire client and malformed/stale/NaN tests.
2. Review native-input preservation in pad polling, especially Move takeover, timeout and menu handling.
3. Review T, Select, gyro axes, bias history and connection-generation resets separately from artwork.
4. Review Nav PS edge detection and the current first-player/global Wii configuration scope. Do not advertise independent per-player attachments without extending and testing that configuration boundary.
5. Review UI changes and translations. Existing glyph-based prompt bars may still use controller glyphs even when the explanatory text says Move.
6. Confirm default-off compilation and runtime marker; then perform a full SDK build and the hardware acceptance checklist.

The second-Move channel is named g_trigger_bridge internally for historical reasons; it is a second hand, not the T-trigger channel. Do not rename its network meaning without a compatibility migration.

## Merge choices

The feature branch can be cherry-picked or its source patch applied against the recorded base. Keep its tests/docs with the implementation. Maintainers may split protocol, backend, UI and build-option changes into smaller commits. Do not fold an unrelated raw Bluetooth host into Porpoise to make a missing external service disappear.

Keep the feature opt-in while producer-side DualSense coexistence is unresolved. A passing mock or UI test does not justify enabling it by default. The original controller behavior must remain usable when the flag is absent, marker is absent, service is absent, packets stop, or data is invalid.

## Rollback

Build without PORPOISE_MOVE_BRIDGE; remove the runtime marker through the normal file-management workflow if appropriate; stop the external service normally. Do not erase stored Bluetooth pairing/calibration files. If the external raw host has wedged native reconnection, reported recovery is rest mode/restart; this is not a consumer-only rollback.
