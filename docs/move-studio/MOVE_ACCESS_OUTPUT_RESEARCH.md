# Native Move and Access output research

2026-10-26. Research only: neither output mode is implemented. The running
Move + Nav DualSense beta is unchanged by this investigation.

## What was checked on firmware 13.60

`tools/output-api-inventory.c` loaded Mbus, Pad and Move in its own process,
then checked export availability. All checked symbols were present:

- Move: Init, Open, ReadStateLatest, ReadStateRecent, GetDeviceInfo,
  SetLightSphere, SetVibration.
- Pad: IsMoveConnected, IsMoveReproductionModel, GetSphereRadius,
  ReadForTracker, SetVirtualDeviceComponentInformation.
- Access/Leonardo: GetControllerTypeLeonardo, SetControllerTypeLeonardo,
  GetProfileLeonardo, SetProfileLeonardo, ParseUniqueDataLeonardo.
- Co-pilot: IsCoPilot, SetIsCoPilot.

The tool called none of those controller APIs. Export presence establishes
neither a function signature nor support for changing an arbitrary device into
that class. No device-type sweep, real-controller mutation, shell patch, shared
radio read, pairing change or physical-controller test was performed.

## Move: three different integration targets

1. **Native system device:** requires the correct virtual descriptor/device
   class, report conversion, user binding and lifecycle. Our known DualSense
   type value 3 is not evidence of a Move type. No verified Move creation API
   or descriptor was found in the inspected projects.
2. **Application compatibility bridge:** REMove replaces `sceMove` operations
   within a PS4 game. PoorDS4 demonstrates the separate PS5 application-input
   forwarding approach. A PS5 port would need firmware-checked hook locations,
   module lifecycle and error behavior; a PS4 plugin is not a drop-in PS5 payload.
3. **Emulator integration:** consume Move Studio's MVB1 streams directly and
   implement guest APIs, such as RPCS3 CellGem. This avoids depending on the
   PS5 shell recognizing a virtual Move, but is not system-wide spoofing.

REMove provides concrete semantics for Open/Close, two controller indices,
latest/history reads, timestamps/counters, digital Move/T/face buttons,
analog T, acceleration, angular velocity, sphere information, RGB and rumble.
Its README explicitly limits the implementation to the game process and says
camera MoveTracker titles are unsupported. Our physical sphere/camera inputs
could help a future tracker integration, but image-space coordinates do not
establish Sony tracking pose semantics or complete 6DoF.

For an application prototype: adapt the verified MVB1 decoder, maintain
per-user/per-hand handles, reject stale input, return disconnected states on
loss, retain read history, and route light/vibration requests back to the
producer. Validate the PS5 Move ABI before sharing any PS4 structure layouts.
The standalone feedback receiver remains missing; do not claim rumble works
through this proposed adapter.

## Nav as Access

Access is a distinct device, not merely a DualSense with fewer buttons.
The ps-access project documents hardware identity 054c:0e5f, USB profile feature
reports 0x60/0x61, three stored profiles, raw physical button fields, mapped
actions, expansion ports and a profile selector. Its hardware input work uses
a DualSense-style report with separate physical and mapped state.

A Nav has a suitable stick and button subset, but matching its controls is
only the input-mapping portion. Still required for actual Access recognition:

- Identify the virtual device class/subtype and component metadata accepted by
  the PS5 Access path, without guessing enum values.
- Verify Leonardo/co-pilot function ABIs and distinguish profile operations
  on an existing Access device from virtual-device creation.
- Define physical-button identity, mapped actions, active profile and any
  required feature responses; confirm whether the virtual Pad interface even
  exposes these fields.
- Test the PS5 Access setup screen, profile selection/remapping, combination
  with another controller, disconnect and cleanup. Ordinary game input alone
  would not prove Access recognition.

Do not advertise Access support merely because Nav input can be represented
as part of a virtual DualSense. The Access USB protocol documentation is useful
reference data, not evidence of a PS5 software-only Access emulator.

## Other useful implementation leads

FGG-XSense describes direct `/dev/hid` virtual registration and distinguishes
its kernel input report from ScePadData. It reports acceptance of eleven
device types on firmware 11.60, but does not identify a working Move/Access
type or supply their semantics. Treat this as a potential route to investigate,
not proof for firmware 13.60. No raw HID backend was added or tested here.

Ghostpad and OmniPad offer virtual DualSense/profile-assignment research;
their shell patching does not establish Move or Access output. Keep these
experiments independent of the current external-radio coexistence behavior.

## Sources and provenance

Research references only; no source implementation copied into an output adapter:

- [REMovePS4](https://github.com/nekohaku/REMovePS4): remove/scemove.h,
  remove/remove_module.c, remove/remove_ctx.c, README limitations.
- [PoorDS4](https://github.com/ItsBlurf/PoorDS4): native input forwarding,
  application attachment, lifecycle and handle recovery.
- [ps-access](https://github.com/johnhenry/ps-access), revision
  37119cbbc3615610b1136079cc97a534d6b57844: PROTOCOL.md. Credits Jacek
  Fedorynski's earlier Access editor. Describes real-hardware USB/profile work.
- [FGG-XSense](https://github.com/FGGstore/FGG-XSense), revision
  bfc5f9d324e2ca8b0a90fc498957699caa9fe5b0: src/vpad.c, src/vpad.h, README.
- [Ghostpad](https://github.com/srbraboo/Ghostpad), revision
  f6c75494d00e4362052071b3fabe88c40e9b6203: virtualDS5research.md and payload.
- [OmniPad](https://github.com/diegobarbosaa/OmniPad-PS5): existing research
  reference recorded in VIRTUAL_OUTPUT_BETA.md.

Any future copied or adapted implementation must retain applicable notices
and update CREDITS.md. Native module bytes, private runtime identities and
controller profiles are not part of this developer document.
