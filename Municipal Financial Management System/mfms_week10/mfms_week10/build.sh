#!/bin/sh
# Separate compilation (-c) of every module, then link into ./mfms
set -e
FLAGS="-std=c99 -Wall -Wextra -pedantic"
for f in main employees budget utilities reports suppliers assets storage; do
    gcc $FLAGS -c $f.c
done
gcc main.o employees.o budget.o utilities.o reports.o suppliers.o assets.o storage.o -o mfms
echo "Build successful: ./mfms"
