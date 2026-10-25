# Compatibility and known issues

Snapshot: 2026-10-24, Move Studio beta6.2 / experimental Porpoise integration. Hardware observations are from a jailbroken PS5 on 13.60. This is a developer handoff, not a finalized compatibility release. A passing mock or synthetic test does not establish hardware support.

## Hardware exercised

| Device / feature | Evidence and limits |
| --- | --- |
| PS3 Move (ZCM1) | Buttons, analog T mapped to Wii B, tilt aiming without T, and Select recenter confirmed in development gameplay. Long sessions and all games remain unverified. |
| PS3 Navigation controller | USB and wireless reports and gameplay exercised. Repeated reconnect, long sessions and multiplayer combinations need validation. Red LED can still blink while connected. |
| Native DualSense | Porpoise buttons/sticks/triggers and menu input exercised. Bluetooth reconnect is broken with the bridge active; see below. |
| PS3 Eye | Sphere tracking and preview exercised. User reported good tracking; wall lights can produce false positives. |

## Untested or only partially tested

| Device / scenario | Current status |
| --- | --- |
| PS4 DualShock 4 and other PS4 controllers | Not hardware-validated in this integration. Existing upstream profiles or research into PoorDS4 do not prove passthrough, native PS5 compatibility, motion, touchpad, rumble or reconnect here. |
| Other Move models, including ZCM2 | No compatibility certification; report layouts/calibration and connection behavior need separate validation. |
| Generic USB UVC webcams | Backend builds and mock discovery/format/disconnect tests pass. No real UVC camera stream has been validated on the PS5. |
| Android phone in USB webcam mode | Requires the phone to expose a UVC video interface. The observed S23 Ultra USB configuration exposed no UVC camera interface. Samsung Camera Share alone did not establish UVC support. |
| Android phone over HTTP MJPEG | PS5 decoded a generated test stream. Actual phone app capture, permissions, network latency and stability remain untested. The receiver accepts a configured private-LAN HTTP endpoint; this is not a verified direct USB-phone camera path. |
| Other cameras / PS4 or PS5 camera accessories | Not validated; do not assume compatibility from the PS3 Eye or UVC implementation. |
| Eight simultaneous physical controllers | Eight software slots and four output players have synthetic tests. Eight real internal-radio links have not been demonstrated. |
| Four-player gameplay and two Moves per player | Routing/assignment tests exist; complete hardware gameplay, reconnection and reassignment combinations are not validated. |
| All tracking colors simultaneously | Some multicolor hardware testing exists; eight-sphere discrimination and occlusion recovery remain unverified. |
| Other firmware / sleep and resume | No general firmware guarantee. Full lifecycle and recovery testing remains outstanding. |

## Currently broken, disabled or missing

- **Native DualSense reconnect:** after turning it off while the bridge runs, it may fail to reconnect. Stopping the bridge alone did not restore it in reported tests; rest mode or reboot did. Beta6.2 restores operational Move/Nav behavior, not a fix for this regression. This blocks a finalized release.
- **System-wide virtual DualSense mode:** guarded off. Native service/module startup previously stopped the payload. Move+Nav-as-DualSense is not ready for normal PS5 menus or retail games.
- **Standalone PS5 app tile:** launcher source exists, but installation/service loading is unresolved; no working standalone tile is claimed.
- **End-to-end vibration:** the Porpoise consumer sends feedback, but the current standalone producer lacks its receive/expiry path. Earlier API acceptance does not prove felt game vibration.
- **Optical drift correction / full pose:** camera observations are 2D pixels and apparent sphere size. Integrated camera drift correction, validated 3D/6DoF pose and guest camera emulation are unfinished. The exported quaternion is currently identity.
- **False tracking targets:** wall lights can resemble spheres. Exclusion regions pass synthetic/API tests; real room setup and moving-camera behavior need validation. A real controller inside an excluded region is also ignored.
- **Nav attachment toggle:** the current Porpoise core path toggles the first player's global Wii extension configuration; independent per-player toggles are not implemented.
- **RPCS3 / PS3 emulator integration:** documentation and protocol plan only. No completed RPCS3 controller/camera integration is included.

## Before enabling by default

Follow [the Porpoise acceptance checklist](../move-bridge/VALIDATION.md), including full native build/link with the feature off and on, native controller coexistence, physical multiplayer, stale-input recovery and sleep/resume. Keep this file and [STATUS.md](STATUS.md) updated with actual evidence when a status changes.

### Bluetooth isolation update — 2026-10-25

After rest-mode recovery and a confirmed working native reconnect baseline, a finite probe that only opened/held the internal Bluetooth USB device with O_RDWR reproduced the unsupported-device rejection. It sent no USB ioctls or Bluetooth commands and queued no reads. The kernel mechanism and recovery after close remain unproven. This strengthens the coexistence blocker; it is not a fix.
