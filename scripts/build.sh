#!/bin/bash

set -x

mkdir -p build
cd build
cmake ..
make

cd ..

# python3 "cat.py" "${@:1:3}" | ./build/peruser
