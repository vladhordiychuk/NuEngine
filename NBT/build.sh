#!/bin/bash

set -e

echo "[NBT] Compiling for Linux/macOS..."

mkdir -p build

g++ \
    -std=c++20 \
    -g \
    -o build/nbt \
    src/main.cpp \
    src/Core/BuildSystem.cpp \
    src/Core/Module.cpp \
    src/Generators/VSGenerator.cpp \
    src/Generators/MakefileGenerator.cpp \
    src/Generators/NinjaGenerator.cpp \
    -I include \
    -I thirdparty/toml11/include \
    -pthread 

echo ""
echo "=============================================="
echo " NBT built successfully: ./build/nbt"
echo "=============================================="
echo ""
echo "Now run:"
echo "  ./build/nbt build ../Game/Game.nuproject"
echo ""