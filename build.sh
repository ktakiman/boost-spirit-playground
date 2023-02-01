#!/bin/bash

set -e

if [ ! -d ninja-clang-debug ]; then
  mkdir ninja-clang-debug
  cd ninja-clang-debug
  cmake -D CMAKE_BUILD_TYPE=Debug -G Ninja -D CMAKE_CXX_COMPILER=clang++ -D CMAKE_EXPORT_COMPILE_COMMANDS=ON ../src/
  if [ $@ != 0 ]; then
    cd -
    rm -rf ninja-clang-debug
    exit
  fi
  cd -
fi

cmake --build ninja-clang-debug


