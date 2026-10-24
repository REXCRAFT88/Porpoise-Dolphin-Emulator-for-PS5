#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/../.."
cxx="${CXX:-clang++}"
mkdir -p build/move-bridge-tests
flags=(-std=c++20 -O2 -Wall -Wextra -Werror)
"$cxx" "${flags[@]}" tests/move_bridge/wire_protocol.cpp -o build/move-bridge-tests/wire_protocol
build/move-bridge-tests/wire_protocol
"$cxx" "${flags[@]}" -DPORPOISE_MOVE_BRIDGE -Ithird_party/libretro tests/move_bridge/pad_integration.cpp src/porpoise_pad.cpp src/porpoise_aim.cpp -pthread -Wl,--wrap=fopen -o build/move-bridge-tests/pad_integration
build/move-bridge-tests/pad_integration
"$cxx" "${flags[@]}" -Ithird_party/libretro -c src/porpoise_pad.cpp -o build/move-bridge-tests/pad_disabled.o
printf '%s\n' 'Move bridge protocol/integration and default-disabled backend compile passed.'
