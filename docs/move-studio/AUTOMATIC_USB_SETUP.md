# Automatic external USB setup — beta6.6

Plug a ZCM1 Move or Navigation controller into USB while the external bridge is ready. The service scans about every two seconds. It identifies the physical controller, preserves its original host report per identity, writes/verifies the selected adapter host when needed, and reads/caches Move calibration. PS5 notifications distinguish USB detected, ready to unplug, and setup failure. Unplug only after the ready notification, then press PS. Duplicate scans avoid repeat writes when the host already matches. Other Sony models and generic gamepads are not automatically repointed.

The worker shares the external transport's initialized USB runtime; it does not initialize/terminate that runtime independently. It is joined before transport close, manual retry or removal recovery. There is no internal-radio fallback.

If the external adapter is removed or becomes unresponsive, links are closed and the worker stops. The bridge retries after three seconds and backs off ten seconds after each five-attempt batch. The dashboard remains available. Reinsert the same adapter and press PS on controllers when the bridge reports ready. Adapter host addresses and persisted Bluetooth keys are not interchangeable; changing physical adapters can require USB setup again. A Nav may still require a full controller power cycle after a bridge restart.

Validation: SDK build and transport mock tests pass. Registration tests exercise real backup/read/write helper code with mocked USB: preserved backup, idempotent repeat scan, identity mismatch refusal, readback failure rollback. Physical plug/reinsert and new automatic notifications still need user validation. No claim of broad adapter hardware validation or native system Bluetooth reset is made.
