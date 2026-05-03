#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cmake -S "$ROOT/host" -B "$ROOT/build/host" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build "$ROOT/build/host"
