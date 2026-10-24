# Validation and acceptance

## Host checks shipped in this branch

Run `CXX=clang++ bash tests/move_bridge/run.sh` from the repository root.

- Wire client: packet versions/sizes, invalid flags, non-finite fields, stale/future/duplicate data, disconnect, producer restart after timeout, and optical freshness.
- Actual pad backend with platform stubs: Move A, T/B, face buttons, independent gyro aiming, recenter/calibration, pointer loss/recovery, Nav menu directions/buttons, attachment-toggle edges, native DualSense buttons/sticks/triggers and timeout release.
- Independent synthetic input on player channels 2..4, in addition to the player-1 behavior checks.
- Consumer feedback datagrams are emitted; this tests the sender, not a physical motor.
- Default-disabled pad backend compiles under strict warnings. An unused-variable issue discovered during packaging was fixed by placing bridge-only state behind its feature guard.

The integration test's final legacy text mentions Circle B; the body also tests the actual T/B path. Test assertions, not the abbreviated output label, define coverage.

## Hardware observations from development

On the development PS5 (13.60), the user confirmed T as Wii B and tilt aiming without T in an experimental Porpoise build. Move/Nav and native menu inputs were exercised. Some earlier builds had setup visibility, recenter, stale-input and controller recovery problems; do not collapse all observations into a release certification.

The latest UI/Nav artwork had been built locally but had not been installed as a final release. This branch is a source handoff, not a claim that every file here has already been played on hardware. The new four-player assertions are synthetic; eight physical internal-radio connections remain unverified.

## Required before default enablement/upstream release

- Full PS5 SDK compile/link/sign of the host with the feature both off and on.
- No bridge service and no marker: ordinary Porpoise behavior.
- Native DualSense navigation before, during and after a session; disconnect and reconnect it while producer is active.
- Every Move button and analog T independently; all intended grips/axes; long stationary drift test and Select recenter.
- Nav repeated C/Z, stick, menu navigation, PS attachment toggle in a game that actually supports the extension.
- Explicit Nunchuk detach prompts and per-game controller overrides.
- All player ports, second-hand routing, reassignment, identity/slot reuse, optical occlusion and input timeout.
- Producer restart, process shutdown, sleep/resume and recovery without stuck buttons or rumble.
- Real feedback receiver and physical motor verification before claiming vibration.

The unresolved producer-side DualSense reconnect bug blocks a finalized/default-on release even if the consumer tests pass.
