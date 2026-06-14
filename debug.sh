#!/bin/bash
export PATH=/usr/bin:$PATH
export DISPLAY=:0
files=(./"Source Files"/*.c)
gcc -fsanitize=address -g main.c "${files[@]}" -o program -lm
./program