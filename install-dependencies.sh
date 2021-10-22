#!/bin/bash

set -e

sudo echo "[Running $0]"

cmd="$1"
if [ -z "$cmd" ]; then
    cmd=install
fi

if which g++-11 2> /dev/null; then
    compiler="g++-11"
else
    compiler="g++"
fi

if [ -z "$source_dir" ]; then
    if [ -d "$HOME/.local" ]; then
        source_dir="$HOME/.local/sources"
    else
        source_dir="$HOME/apps/sources"
    fi
fi
echo Set source directory to $source_dir

function install() {
    user=$1
    project=$2
    mkdir -p $source_dir
    cd $source_dir

    if [ -d $project ]; then
        cd $project
        git pull
    else
        git clone "https://github.com/$user/$project.git"
        cd $project
    fi
    

    cmake -B build                               \
        -DCMAKE_BUILD_TYPE=Release               \
        -DCMAKE_CXX_FLAGS="-std=c++20 -fPIE"     \
        -DBUILD_TESTING=OFF                      \
        -DCMAKE_CXX_COMPILER="$compiler"

    cmake --build build -j 16
    
    sudo cmake --install build
}
function update() {
    install "${@}"
}

function uninstall() {
    user=$1
    project=$2
    cd "$source_dir/$project"

    # This needs to run on mac and I'm not sure mac has the --arg-file flag for xargs 
    cat build/install_manifest.txt | sudo xargs rm -f -v
    cd ..
    rm -rf "$project"
}


$cmd catchorg  Catch2
$cmd fmtlib    fmt
$cmd codeinred tuplet
$cmd codeinred recursive-variant

