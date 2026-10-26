# Active work list

Updated 2026-10-25. Keep this list aligned with actual implementation and hardware evidence.

## Bluetooth

- [ ] Test the external-only ASUS build after a clean PS5 restart and verified native DualSense reconnect baseline. User is not ready to restart yet; old bridge is stopped.
- [ ] Migrate a deliberately selected Move/Nav to the external adapter through USB, backing up and verifying original host addresses. Do not repoint every attached controller automatically.
- [ ] Validate input, output, reconnect, unplug/reinsert, multiplayer and native DualSense coexistence with the external backend.
- [ ] Find a proven native Bluetooth recovery mechanism after internal raw-device access. A client HID Term/Init is not proven to restart the system service; rest mode has not recovered every observed failure.

## Controller profile artwork

- [ ] In the **Wii Remote + Nunchuk** profile preview, show a **Navigation controller beside the Move controller**. Include both in the combined preview rather than showing Move alone. Keep the Wii Remote-only preview as Move alone; verify layout at phone and PS5 browser sizes. Requested by the user on 2026-10-25; not implemented yet.

## Camera

- [ ] Validate beta6.4 phone tracking and frame stability with moderate-resolution IP Webcam settings; keep automatic fallback.
- [ ] Identify the exact field behind the earlier MJPEG format rejection rather than attributing every drop to Wi-Fi or resolution.
