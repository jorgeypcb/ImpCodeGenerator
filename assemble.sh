#!/bin/bash

for file in programs/*.imp; do 
    build/assembler $file     \
        1> ${file%.imp}.ins   \
        2> ${file%.imp}.vars
done
