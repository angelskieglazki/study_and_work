#!/bin/bash

cmake -DCMAKE_TOOLCHAIN_FILE=x86-linux-clang-14.cmake -S ./ -B build
cmake --build build/