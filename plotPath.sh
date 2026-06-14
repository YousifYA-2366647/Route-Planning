#!/bin/bash
export PATH=/usr/bin:$PATH
export DISPLAY=:0
files=(./"Source Files"/*.c)
gcc main.c "${files[@]}" -o program -lm
./program | /usr/bin/python3 ./pathPlotter.py