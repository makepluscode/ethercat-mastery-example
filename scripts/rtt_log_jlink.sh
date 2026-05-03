#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
ELF="$ROOT/firmware/build/APP_KIT_XMC48_RELAX_ECAT_V1/Debug/mtb-example-xmc48-ethernet-ethcat-loopback.elf"
TC="${ARM_GNU_TOOLCHAIN_BIN:-$ROOT/tools/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi/bin}"
JLINK_DIR="$ROOT/tools/jlink/JLink_Linux_V940_x86_64"
OUTFILE="$(mktemp -p /tmp jlink-rtt.XXXXXX.log)"

if [[ -f "$HOME/.env" ]]; then
  # shellcheck disable=SC1090
  source "$HOME/.env"
fi

if [[ ! -x "$JLINK_DIR/JLinkExe" ]] || [[ ! -x "$JLINK_DIR/JLinkRTTLoggerExe" ]]; then
  echo "J-Link tools not found in $JLINK_DIR" >&2
  exit 1
fi

if [[ ! -f "$ELF" ]]; then
  echo "Firmware ELF not found at $ELF" >&2
  exit 1
fi

addr="$("$TC/arm-none-eabi-nm" "$ELF" | awk '/ _SEGGER_RTT$/ {print "0x"$1; exit}')"
if [[ -z "$addr" ]]; then
  echo "SEGGER RTT control block not found in $ELF" >&2
  exit 1
fi

JLINK_LOAD=/tmp/jlink-load.jlink
cat > "$JLINK_LOAD" <<EOF
loadfile $ELF
r
g
exit
EOF

trap 'rm -f "$OUTFILE" "$JLINK_LOAD" /tmp/jlink-logger.stdout; pkill -f "$JLINK_DIR/JLinkRTTLoggerExe" 2>/dev/null || true' EXIT

if [[ -n "${SUDO_PASSWORD:-}" ]]; then
  printf '%s\n' "$SUDO_PASSWORD" | sudo -S -p '' "$JLINK_DIR/JLinkExe" -device XMC4800-F144x2048 -if SWD -speed 1000 -CommanderScript "$JLINK_LOAD" >/tmp/jlink-load.stdout 2>&1
else
  "$JLINK_DIR/JLinkExe" -device XMC4800-F144x2048 -if SWD -speed 1000 -CommanderScript "$JLINK_LOAD" >/tmp/jlink-load.stdout 2>&1
fi

sleep 2

LOGGER_CMD=("$JLINK_DIR/JLinkRTTLoggerExe" -Device XMC4800-F144x2048 -If SWD -Speed 1000 -RTTAddress "$addr" "$OUTFILE")
if [[ -n "${SUDO_PASSWORD:-}" ]]; then
  LOGGER_CMD=(sudo -S -p '' "${LOGGER_CMD[@]}")
  printf '%s\n' "$SUDO_PASSWORD" | "${LOGGER_CMD[@]}" >/tmp/jlink-logger.stdout 2>&1 &
else
  "${LOGGER_CMD[@]}" >/tmp/jlink-logger.stdout 2>&1 &
fi
logger_pid=$!

for _ in $(seq 1 100); do
  if [[ -s "$OUTFILE" ]]; then
    break
  fi
  if ! kill -0 "$logger_pid" 2>/dev/null; then
    cat /tmp/jlink-logger.stdout >&2
    exit 1
  fi
  sleep 0.1
done

tail --pid="$logger_pid" -f "$OUTFILE"
