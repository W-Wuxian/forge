#!/bin/bash

SCRIPT_DIR=$(dirname "$(realpath "$0")")

echo "SCRIPT DIR IS $SCRIPT_DIR"
cd $SCRIPT_DIR

if command -v guix >/dev/null 2>&1; then
    echo "Guix is installed."
    comp="guix shell gcc-toolchain@14.2.0 -- gcc "
else
    echo "Guix is not installed."
    comp=gcc
fi

#FORGE_ROOT=$HOME/Work/github/Forge
FORGE_ROOT=$PWD/..
export CPATH=$FORGE_ROOT:$FORGE_ROOT/fwht_utils:$FORGE_ROOT/fwht
echo $CPATH

mkdir -p bin/
rm -f bin/*

# showcase fwht
$comp -O2 -Wall -o bin/showcase_fwht $FORGE_ROOT/fwht_utils/hada.c fwht.c test/fwht_showcase.c -DM=16 -I$CPATH 2>&1 | tee build.log
#guix shell julia -- julia julia_hadamardize.jl

$comp -O2 -Wall -o bin/test_base_dummy_fwht \
$FORGE_ROOT/fwht_utils/hada.c \
fwht.c test/fwht_tester.c test/fwht_tester_base_dummy_fwht.c \
-I$CPATH \
2>&1 | tee -a build.log
