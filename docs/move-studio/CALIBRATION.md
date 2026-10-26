# Calibration menu

Beta6.10 adds a Calibration page for each connected ZCM1 Move. Place the
controller on a stable surface, release all buttons, select its player/device,
then select Start calibration. The page shows progress and completion/error.

The sampler requires three seconds and at least 100 input reports, rejects
rotation above 0.15 rad/s or acceleration changing by over 0.08 g on any axis,
and fails on input loss or a five-second deadline. Movement rejection preserves
the previous bias. This assumes the user follows the stationary instructions;
slow constant rotation cannot reliably be distinguished from gyro bias.

The measured raw gyro mean is applied at the existing conversion step to both
emulator packets and combined virtual output. Reset removes only that runtime
correction. Reconnecting resets it too; factory calibration, pairing and layouts
are untouched. No neutral game input is synthesized during sampling.

The page links aiming settings and camera setup and explains Select recentering
inside the emulator. It does not claim to calibrate screen geometry, orientation,
optical pose or Nav stick hardware. Current factory scaling remains required;
connect by USB if it is unavailable. Multi-device selection uses controller
identity, and disconnected/incorrect device requests are rejected.

Host tests cover zero-rate correction, motion rejection preserving old bias,
silent-device timeout, reset, disconnection and API request validation. Native
build and live UI/API checks are separate from hardware stationary calibration
and aiming verification, which require a user's controller test.
