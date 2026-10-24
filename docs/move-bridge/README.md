# Move / Navigation integration handoff

This branch is an experimental, opt-in input integration for Porpoise. It is based on upstream Main commit `db6c13e20a9e4c3e16e088ecb8b3d0098efa73b2`. It does not replace Porpoise's Bluetooth implementation or install the external bridge.

Start here:

- [Maintainer integration and rollback](MERGING.md)
- [Validation and hardware acceptance](VALIDATION.md)
- [AI assistant handoff](AI_HANDOFF.md)
- [Source attribution](CREDITS.md)
- [Shared producer/consumer protocol](../move-studio/PROTOCOL.md)
- [Standalone bridge status and limitations](../move-studio/STATUS.md)

## Behavior

Move input is received from an external local service using validated loopback UDP. T maps to Wii B, Move maps to A, Cross to 1, Square to 2, Triangle to Plus; existing Circle-to-B behavior is retained as an alternate. Select requests recenter. Gyro aiming does not require T. Nav provides menu/navigation input and Nunchuk stick/C/Z. Nav PS can request the Wii Remote/Nunchuk toggle for the first player through the current core configuration path.

Native DualSense menu buttons/sticks/triggers remain available alongside Move. Its gyro is excluded from Move aiming. The setup UI uses Move/Nav artwork and Move-button prompts. Motion uses calibrated bridge sensors with stationary-bias calibration and reconnect/timeout handling.

The external producer currently causes native DualSense reconnection failures. This fork's consumer does not fix that producer regression. Camera input is observations only; full optical drift correction, guest camera emulation and 3D pose are not implemented. The consumer sends vibration feedback, but the current standalone producer lacks a receive path; felt end-to-end vibration is not claimed.

## Default off

The source integration is disabled unless `PORPOISE_MOVE_BRIDGE` is compiled in. Socket setup is additionally gated by the runtime marker `/data/porpoise-move-bridge.enable`.

For a complete experimental build, first follow upstream [BUILDING.md](../../BUILDING.md), then opt in with:

```sh
PORPOISE_MOVE_BRIDGE=1 make porpoise
```

Normal `make porpoise` does not add the definition. The upstream build pipeline may cache objects; perform a clean/relevant rebuild when toggling the flag. Build/install under an explicitly chosen experimental title identity using the upstream supported process; this handoff does not replace the user's installed original app or hardcode a private console address.

## Run tests without a console or payload SDK

```sh
CXX=clang++ bash tests/move_bridge/run.sh
```

This compiles/runs the wire client and actual pad backend against platform stubs, then compiles the default-disabled backend. It does not establish full native application build or hardware gameplay compatibility on its own.
