#!/bin/bash

# Move to the root directory of the project
cd $(dirname $0)

# Fetch packages and statically link
./build.sh -DALWAYS_FETCH=ON -DCMAKE_CXX_FLAGS="-static"

# Create package directory and copy build/execute into package directory
mkdir -p package
mkdir -p package/programs 

cp build/execute package
cp programs/* package/programs
