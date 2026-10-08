#!/bin/bash
set -e

: "${OPENORBIS:=${OO_PS4_TOOLCHAIN:-}}"
export OPENORBIS
: "${OPENORBIS:?OpenOrbis toolchain environment not found}"

ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
BUILD="$ROOT/build-ps4"

if [ "$1" = "clean" ]; then
    rm -rf "$BUILD"
fi

cmake -S "$ROOT" -B "$BUILD" \
    -DCMAKE_BUILD_TYPE=Release

cmake --build "$BUILD" -j2

echo
echo "=== Fallout 1 PS4 build artifacts ==="
find "$BUILD" -maxdepth 1 \( -name '*.pkg' -o -name 'eboot.bin' \) -print
