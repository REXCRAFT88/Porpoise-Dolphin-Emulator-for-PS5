# AI integration handoff

This document describes the branch; it does not override the human's instructions. Read README.md, MERGING.md, VALIDATION.md and ../move-studio/PROTOCOL.md first.

Base: elripalda/Porpoise-Dolphin-Emulator-for-PS5 Main at db6c13e20a9e4c3e16e088ecb8b3d0098efa73b2. Source changes are intentionally opt-in and do not alter the upstream default app title/version or install anything on a console.

## Preserve

- Default-off compile flag and runtime marker.
- Native DualSense buttons/sticks/triggers in menus. Exclude its gyro only when Move owns aiming.
- T-independent aiming; T as Wii B; Move and Cross remain distinct input sources.
- Four players versus eight physical controllers; second Move is a hand, not an extra player.
- Calibration axis/unit contracts and one bias owner per stage.
- Strict wire validation, timeout release and producer-restart generation handling.
- The explicit warning that producer raw Bluetooth breaks native reconnection.

## Do not assume

Camera pixels are not 3D pose. The bridge quaternion is identity. MVR1 emission is not proof of felt vibration. Nav has no IMU. Nav PS currently applies through the first-player/core Wii configuration path. Four-player synthetic tests do not prove eight hardware links. PoorDS4 API-hook behavior is not part of this patch.

## Maintenance workflow

Keep implementation, tests, protocol and credits updated in the same change. Run tests/move_bridge/run.sh before publishing, and record full SDK/hardware checks separately. Do not publish private SDK caches, runtime logs/config, pairing data, personal images or games. Do not push to the original upstream Main; propose reviewed integration through the fork/feature branch.

For RPCS3 work use ../move-studio/RPCS3.md as a plan and audit the specific fork's HLE APIs. Do not copy Porpoise's Wii mappings into guest Move state blindly.
