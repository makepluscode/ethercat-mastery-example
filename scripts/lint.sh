#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

files=(
  host/include/ecat/ethercat.hpp
  host/include/ecat/raw_socket.hpp
  host/src/ethercat.cpp
  host/src/main.cpp
  host/src/raw_socket.cpp
  firmware/src/main.cpp
  firmware/src/ethernet/eth_rx.cpp
  firmware/src/ethernet/eth_rx.hpp
  firmware/src/logging/log.hpp
  firmware/src/platform/retarget_stdio.cpp
)

clang-format --dry-run --Werror "${files[@]}"

if [[ -d build/host ]]; then
  clang-tidy --quiet -p build/host host/src/main.cpp host/src/ethercat.cpp host/src/raw_socket.cpp
else
  echo "skip host clang-tidy: run scripts/build_host.sh first"
fi
