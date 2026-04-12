#!/bin/bash

set -x

cd ~/prjs/peruser/build

cmake ..
make

cd ..

# python3 "cat.py" "${@:1:3}" | ./build/peruser
