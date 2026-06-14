#!/bin/bash
export PATH=/usr/bin:$PATH
export DISPLAY=:0
files=(./"Source Files"/*.c)
gcc -O3 main.c "${files[@]}" -o program -lm
./program