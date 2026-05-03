#!/usr/bin/env python3
"""Rewrite a ModusToolbox-generated ninja file from Windows paths to Linux."""
import argparse
import re
import sys
from pathlib import Path


def main() -> int:
    p = argparse.ArgumentParser()
    p.add_argument("--src", required=True, help="Input .ninja (Windows-generated)")
    p.add_argument("--out", required=True, help="Output .ninja path")
    p.add_argument("--project", required=True, type=Path, help="App root (xmc4800)")
    p.add_argument("--mtb-shared", required=True, type=Path, help="mtb_shared directory")
    p.add_argument(
        "--toolchain-bin",
        required=True,
        type=Path,
        help="Directory containing arm-none-eabi-gcc",
    )
    args = p.parse_args()

    proj = args.project.resolve()
    shared = args.mtb_shared.resolve()
    tcbin = args.toolchain_bin.resolve()

    for name in ("arm-none-eabi-gcc", "arm-none-eabi-g++", "arm-none-eabi-ar"):
        if not (tcbin / name).is_file():
            print(f"error: missing {tcbin / name}", file=sys.stderr)
            return 1

    text = Path(args.src).read_text(encoding="utf-8", errors="replace")

    text = text.replace("__PROJECT__", str(proj))
    text = text.replace("__MTB_SHARED__", str(shared))
    text = text.replace("__TOOLCHAIN_BIN__", str(tcbin))
    text = text.replace("../mtb_shared", str(shared))
    text = text.replace(".exe", "")

    win_exe = re.compile(r"C\$:[^ \r\n]*?arm-none-eabi-(gcc|g\+\+|ar)")

    def sub_exe(m: re.Match[str]) -> str:
        tool = m.group(1)
        if tool == "g++":
            return str(tcbin / "arm-none-eabi-g++")
        if tool == "gcc":
            return str(tcbin / "arm-none-eabi-gcc")
        return str(tcbin / "arm-none-eabi-ar")

    text = win_exe.sub(sub_exe, text)

    out = Path(args.out)
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(text, encoding="utf-8")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
