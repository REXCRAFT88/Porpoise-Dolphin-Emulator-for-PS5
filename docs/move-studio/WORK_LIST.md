# Active work list

Updated 2026-10-25. Keep this list aligned with actual implementation and hardware evidence.

## Bluetooth

- [x] Initial external-only ASUS build test: user confirmed DualSense reconnects and navigates normally before Move/Nav migration. Long-session and simultaneous controller tests remain pending.
- [ ] Investigate external adapter becoming silent during idle; restarting its external session restored ready state in the first live bridge test.
- [ ] Fix Nav automatic reconnect after external service restart: initial wireless connection worked, but restart produced HID channel 0x11 setup timeout. Fully powering Nav off and back on recovered it without re-pairing; both channels/profile then opened. Move reconnect and DualSense coexistence passed their initial checks.
- [x] Migrate one deliberately selected ZCM1 Move to ASUS through USB, back up and verify its host, and confirm wireless dashboard connection. Nav migration remains pending; do not repoint every attached controller automatically.
- [ ] Validate input, output, reconnect, unplug/reinsert, multiplayer and native DualSense coexistence with the external backend.
- [x] Initial external shutdown check: Move/Nav disconnected cleanly, external scan setting restored, lock released, and user confirmed DualSense reconnects/navigates without rest mode or reboot. Reattachment, long sessions and adapter removal remain pending.
- [ ] Find a proven native Bluetooth recovery mechanism after internal raw-device access. A client HID Term/Init is not proven to restart the system service; rest mode has not recovered every observed failure.

## Controller profile artwork

- [ ] In the **Wii Remote + Nunchuk** profile preview, show a **Navigation controller beside the Move controller**. Include both in the combined preview rather than showing Move alone. Keep the Wii Remote-only preview as Move alone; verify layout at phone and PS5 browser sizes. Requested by the user on 2026-10-25; not implemented yet.

## Camera

- [ ] Validate beta6.4 phone tracking and frame stability with moderate-resolution IP Webcam settings; keep automatic fallback.
- [ ] Identify the exact field behind the earlier MJPEG format rejection rather than attributing every drop to Wi-Fi or resolution.
