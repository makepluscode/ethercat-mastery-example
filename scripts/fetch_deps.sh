#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
MTB="$ROOT/third_party/mtb_shared"
mkdir -p "$MTB"

clone_tag() {
  local repo="$1"
  local tag="$2"
  local dest="$3"

  if [[ -d "$dest" ]]; then
    echo "skip $dest"
    return
  fi

  git clone --depth 1 --branch "$tag" "https://github.com/Infineon/${repo}.git" "$dest"
  rm -rf "$dest/.git"
}

clone_tag cmsis release-v5.8.0 "$MTB/cmsis/release-v5.8.0"
clone_tag core-lib release-v1.7.0 "$MTB/core-lib/release-v1.7.0"
clone_tag core-make release-v3.9.0 "$MTB/core-make/release-v3.9.0"
clone_tag device-db release-v4.37.0 "$MTB/device-db/release-v4.37.0"
clone_tag mtb-xmclib-cat3 release-v4.7.0 "$MTB/mtb-xmclib-cat3/release-v4.7.0"
clone_tag recipe-make-cat3 release-v2.5.0 "$MTB/recipe-make-cat3/release-v2.5.0"

echo "deps ready: $MTB"
