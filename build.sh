#!/bin/bash
set -e

BUILD_DIR="build"

[ -d "$BUILD_DIR" ] || mkdir "$BUILD_DIR"
cd "$BUILD_DIR"

cmake ..
cmake --build . -- -j$(nproc)
cmake --install . --prefix ..
