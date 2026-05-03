#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
TC_DIR="$ROOT/tools/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi"
TC_URL="https://developer.arm.com/-/media/Files/downloads/gnu/14.2.rel1/binrel/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi.tar.xz"

sudo apt-get update
sudo apt-get install -y \
  ca-certificates clang-format clang-tidy cmake curl gdb-multiarch git \
  make netcat-openbsd ninja-build openocd perl python3

"$ROOT/scripts/fetch_deps.sh"

if [[ ! -x "$TC_DIR/bin/arm-none-eabi-gcc" ]]; then
  mkdir -p "$ROOT/tools"
  tmp="$(mktemp -t arm-gnu-toolchain.XXXXXX.tar.xz)"
  curl -L "$TC_URL" -o "$tmp"
  tar -xJf "$tmp" -C "$ROOT/tools"
  rm -f "$tmp"
fi

echo "setup complete"
