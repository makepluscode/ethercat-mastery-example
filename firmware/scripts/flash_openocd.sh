#!/usr/bin/env bash
# Program XMC4800 Relax kit via OpenOCD (on-board J-Link).
set -euo pipefail
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
APP_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$APP_ROOT"

ELF="${1:-build/APP_KIT_XMC48_RELAX_ECAT_V1/Debug/mtb-example-xmc48-ethernet-ethcat-loopback.elf}"
if [[ ! -f "$ELF" ]]; then
  echo "usage: $0 [path-to.elf]"
  exit 1
fi

ELF_ABS="$(readlink -f "$ELF")"

# Requires USB permissions for SEGGER J-Link (see README Ubuntu section).
exec openocd -f board/xmc4800-relax.cfg -c "program ${ELF_ABS} verify reset exit"
