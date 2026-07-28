#!/bin/bash
cmake -B build -DCMAKE_TOOLCHAIN_FILE=x86_64-linux-toolchain.cmake -D CMAKE_BUILD_TYPE=Debug .
cmake --build build --target freestanding_meta_example