#!/usr/bin/env bash
# Build firmware on Ubuntu without launching the ModusToolbox IDE.
set -euo pipefail
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
APP_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
REPO_ROOT="$(cd "$APP_ROOT/.." && pwd)"
cd "$APP_ROOT"

DEFAULT_TC="$REPO_ROOT/tools/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi/bin"
TC_BIN="${ARM_GNU_TOOLCHAIN_BIN:-$DEFAULT_TC}"
PROJ="$APP_ROOT"
SHARED="$REPO_ROOT/third_party/mtb_shared"
WIN_NINJA="build/APP_KIT_XMC48_RELAX_ECAT_V1/Debug/mtb-example-xmc48-ethernet-ethcat-loopback.ninja"
TEMPLATE_NINJA="$SCRIPT_DIR/linux/mtb-example-xmc48-ethernet-ethcat-loopback.ninja.win-template"
SRC_NINJA="$WIN_NINJA"
if [[ ! -f "$SRC_NINJA" ]]; then
  SRC_NINJA="$TEMPLATE_NINJA"
fi
OUT_NINJA="build/linux/mtb-example-xmc48-ethernet-ethcat-loopback.ninja"
APPNAME="mtb-example-xmc48-ethernet-ethcat-loopback"

if [[ ! -f "$SRC_NINJA" ]]; then
  echo "error: no ninja template; expected $WIN_NINJA or $TEMPLATE_NINJA"
  exit 1
fi
if [[ ! -d "$TC_BIN" ]] || [[ ! -x "$TC_BIN/arm-none-eabi-gcc" ]]; then
  echo "error: Arm GNU Toolchain not found at $TC_BIN"
  echo "Set ARM_GNU_TOOLCHAIN_BIN to the toolchain's bin directory."
  exit 1
fi
if [[ ! -d "$SHARED/cmsis/release-v5.8.0" ]]; then
  echo "error: mtb_shared missing at $SHARED; run scripts/fetch_deps.sh from repo root"
  exit 1
fi

python3 "$SCRIPT_DIR/patch_ninja_linux.py" \
  --src "$SRC_NINJA" \
  --out "$OUT_NINJA" \
  --project "$PROJ" \
  --mtb-shared "$SHARED" \
  --toolchain-bin "$TC_BIN"

rm -rf build/Debug build/APP_KIT_XMC48_RELAX_ECAT_V1/Debug/*.o 2>/dev/null || true
ninja -f "$OUT_NINJA"

ELF="build/APP_KIT_XMC48_RELAX_ECAT_V1/Debug/${APPNAME}.elf"
HEX="build/APP_KIT_XMC48_RELAX_ECAT_V1/Debug/${APPNAME}.hex"
"$TC_BIN/arm-none-eabi-objcopy" -O ihex "$ELF" "$HEX"
echo "OK: $ELF"
echo "OK: $HEX"
