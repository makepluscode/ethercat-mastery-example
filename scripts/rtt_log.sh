#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
ELF="$ROOT/firmware/build/APP_KIT_XMC48_RELAX_ECAT_V1/Debug/mtb-example-xmc48-ethernet-ethcat-loopback.elf"
TC="${ARM_GNU_TOOLCHAIN_BIN:-$ROOT/tools/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi/bin}"

addr="$("$TC/arm-none-eabi-nm" "$ELF" | awk '/ _SEGGER_RTT$/ {print "0x"$1; exit}')"
if [[ -z "$addr" ]]; then
  echo "SEGGER RTT control block not found in $ELF" >&2
  exit 1
fi

openocd -f board/xmc4800-relax.cfg \
  -c "init" \
  -c "reset run" \
  -c "sleep 2000" \
  -c "halt" \
  -c "rtt setup $addr 256 {SEGGER RTT}" \
  -c "rtt start" \
  -c "rtt server start 9090 0" \
  -c "resume" &
ocd_pid=$!
trap 'kill "$ocd_pid" 2>/dev/null || true' EXIT

sleep 1
nc -d localhost 9090
