# AGENT.md

This repository is operated mostly by coding agents. Keep it portable, small, and easy to extend toward EtherCAT PDO.

## Project Goal

- Build a minimal EtherCAT master path.
- Current scope: single-datagram EtherCAT command layer, raw Ethernet scan, WKC detection, basic ESC register reads.
- Next scope: AL state control, SII/mailbox reads, PDO mapping, cyclic PDO exchange.

## Layout

```text
host/       Linux C++17 raw-socket EtherCAT master
firmware/   XMC4800 Relax EtherCAT firmware for board/ESC/RTT logging
scripts/    Setup, build, flash, RTT, lint helpers
tools/      Downloaded Arm toolchain, ignored
third_party/mtb_shared/  Downloaded ModusToolbox libs, ignored
```

## Hard Rules

- Do not use absolute paths in tracked files. Derive paths from script location or CMake variables.
- Do not commit generated build output, downloaded toolchains, or downloaded ModusToolbox libraries.
- Do not leave nested `.git` directories inside this tree.
- Keep host code C++17, small, and split by responsibility.
- Keep firmware code C++17 where possible; XMC/Modus low-level C APIs are expected.
- Prefer root scripts for normal workflows.

## Commit Messages

- Use a prefix.
- Keep the full message between 10 and 20 words.
- Use English.
- Start with one of: `host:`, `firmware:`, `scripts:`, `docs:`, `build:`, `lint:`, `refactor:`, `deps:`.
- State the concrete change, not a vague activity.

Examples:

```text
host: Split EtherCAT scan transport for future PDO state handling
firmware: Log ECAT link status without requiring X400 Ethernet link
docs: Document portable Ubuntu setup and scan workflow
```

## Ignored External State

These paths must remain untracked:

```text
build/
tools/
third_party/mtb_shared/
firmware/build/
```

`scripts/fetch_deps.sh` clones ModusToolbox dependencies and removes their `.git` directories.

## Environment

- EtherCAT NIC: `eno1` (MAC `94:c6:91:a4:78:bb`) — connect ECAT IN port here.
- sudo password is stored in `~/.env` as `SUDO_PASSWORD=...`.

## Common Commands

```bash
./scripts/setup_ubuntu.sh
./scripts/build_host.sh
./scripts/build_firmware.sh
./scripts/lint.sh
./scripts/flash_firmware.sh
./scripts/rtt_log.sh
./scripts/rtt_log_jlink.sh
```

## Script Usage

Use the root-level scripts for normal agent workflows. They derive paths from the repository root and avoid hard-coded
machine-specific paths.

- `./scripts/setup_ubuntu.sh`
  - Installs Ubuntu packages needed for host build, firmware build, formatting, OpenOCD, RTT, and networking tools.
  - Runs `scripts/fetch_deps.sh`.
  - Downloads Arm GNU Toolchain 14.2 into `tools/` when it is missing.
  - Requires `sudo` and network access.
- `./scripts/fetch_deps.sh`
  - Downloads the pinned Infineon ModusToolbox dependencies into `third_party/mtb_shared/`.
  - Removes nested `.git` directories after each clone.
  - Safe to re-run; existing dependency directories are skipped.
- `./scripts/build_host.sh`
  - Configures CMake with `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON`.
  - Builds the Linux host tool into `build/host/`.
  - Run this before `./scripts/lint.sh` if host `clang-tidy` coverage is needed.
- `./scripts/build_firmware.sh`
  - Delegates to `firmware/scripts/build_linux.sh`.
  - Requires `third_party/mtb_shared/` and Arm GNU Toolchain.
  - Respects `ARM_GNU_TOOLCHAIN_BIN` when a non-default toolchain path is needed.
  - Produces `.elf` and `.hex` files under `firmware/build/`.
- `./scripts/lint.sh`
  - Runs `clang-format --dry-run --Werror` on tracked host and firmware source files.
  - Runs host `clang-tidy` only when `build/host/` exists.
  - Does not rewrite files.
- `./scripts/flash_firmware.sh`
  - Delegates to the OpenOCD flash script under `firmware/scripts/`.
  - Requires the XMC4800 Relax EtherCAT kit debug USB and usually `sudo`.
- `./scripts/rtt_log.sh`
  - Finds the SEGGER RTT control block in the firmware ELF with `arm-none-eabi-nm`.
  - Starts OpenOCD RTT server on localhost port `9090` and connects with `nc`.
  - Requires a built firmware ELF, OpenOCD, netcat, and connected hardware.
- `./scripts/rtt_log_jlink.sh`
  - Starts `JLinkRTTLoggerExe` directly against the target and tails the generated RTT log file.
  - Uses the installed J-Link software pack under `tools/jlink/`.
  - Accepts the RTT control block address from the firmware ELF and can use `sudo` when `~/.env` provides a password.
  - Requires J-Link USB access and a connected target.

When `sudo` is required in this environment, use:

```bash
source ~/.env
echo "$SUDO_PASSWORD" | sudo -S <command>
```

## Running the Host Scanner

`ecat_scan_xmc4800` needs `CAP_NET_RAW` to open an `AF_PACKET` raw socket.
Grant it once after each build, then run without sudo:

```bash
source ~/.env
echo "$SUDO_PASSWORD" | sudo -S setcap cap_net_raw+ep build/host/ecat_scan_xmc4800
./build/host/ecat_scan_xmc4800 eno1 8
```

Expected output when XMC4800 is connected and firmware is running:

```text
ecat_scan_xmc4800: interface=eno1 mac=94:c6:91:a4:78:bb
probe: EtherType 0x88a4, raw AF_PACKET
BRD ESC type: wkc=1 value=0x0198
BRD AL status: wkc=1 value=0x0001
BRD DL status: wkc=1 value=0x5617
slaves detected by WKC: 1
slave[1] adp=0x0000 esc_type=0x0198 station=0x0000 alias=0x0000 al=0x0001 dl=0x5617
```

Key register values (confirmed working):
- `esc_type=0x0198` — XMC4800 integrated ESC, revision 1
- `al=0x0001` — AL state: INIT
- `dl=0x5617` — Port 0 link up, PDI operational, enhanced link detection active

## Running the Firmware (RTT Logs)

Flash once, then read RTT output over J-Link:

```bash
source ~/.env
echo "$SUDO_PASSWORD" | sudo -S ./scripts/flash_firmware.sh
./scripts/rtt_log.sh
```

Expected RTT lines:

```text
Boot: cybsp_init OK
ETH/ECAT logs enabled: ECAT IN for WKC scan, X400 for raw RX probe frames.
ECAT: p0_link=... p1_link=... al=... al_event=... status=...
```

## GitHub Issue Format

Use this format when filing a GitHub issue for this repository:

- Title: short, behavior-first, one sentence.
- Body: English only, core facts only.
- Include:
  - Environment: OS, target board, script or tool name, and version when relevant.
  - Tried: exact commands or steps already attempted.
  - Observed: the concrete failure or unexpected behavior.
  - Expected: the behavior that should happen instead.
  - Notes: one or two key log lines, addresses, or workarounds.
- Keep the report tight. Avoid long diagnosis unless it is directly supported by evidence.

Template:

```text
Environment:
- ...

Tried:
- ...

Observed:
- ...

Expected:
- ...

Notes:
- ...
```

## Host Code

```text
host/include/ecat/raw_socket.hpp
host/include/ecat/ethercat.hpp
host/src/raw_socket.cpp
host/src/ethercat.cpp
host/src/main.cpp
```

- `raw_socket`: Linux AF_PACKET transport only.
- `ethercat`: EtherCAT frame/datagram logic and scan model.
- `main`: CLI only.

Keep future PDO work out of `main.cpp`; add small domain files under `host/include/ecat` and `host/src`.

## Firmware Code

```text
firmware/src/main.cpp
firmware/src/ethernet/
firmware/src/logging/
firmware/src/platform/
```

- Firmware is for RTT visibility of board state and incoming raw Ethernet on X400.
- ECAT IN/OUT status is logged through ECAT peripheral registers and link pins.
- Do not block ECAT logging on X400 link state.

## Verification Standard

Before reporting completion after code changes:

```bash
./scripts/build_host.sh
./scripts/build_firmware.sh
./scripts/lint.sh
```

If hardware is connected and relevant, always run the target-level verification for the changed behavior before
reporting completion. For scan/datagram work, run:

```bash
./build/host/ecat_scan_xmc4800 eno1 8
./scripts/rtt_log.sh
```

Remove generated build output before presenting the tree as git-ready unless the user asks to keep artifacts.
