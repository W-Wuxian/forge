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
export CPATH=$FORGE_ROOT:$FORGE_ROOT/fwht_utils

mkdir -p bin/
rm -f bin/*

# showcase hada
$comp -O2 -Wall -o bin/showcase_hada hada.c test/hada_showcase.c -DM=16 -I$CPATH 2>&1 | tee build.log
guix shell julia -- julia julia_hadamardize.jl 2>&1 | tee -a build.log
bin/showcase_hada |& tee -a build.log

# Make test
# test trailing zero trz
$comp -O2 -Wall -o bin/test_trz hada.c test/hada_tester.c test/hada_tester_trz.c -I$CPATH 2>&1 | tee -a build.log
# test base_ilog2
$comp -O2 -Wall -o bin/test_ilog2 hada.c test/hada_tester.c test/hada_tester_ilog2.c -I$CPATH
# test base_ispow2
$comp -O2 -Wall -o bin/test_ispow2 hada.c test/hada_tester.c test/hada_tester_ispow2.c -I$CPATH
# test base_n2p
$comp -O2 -Wall -o bin/test_n2p hada.c test/hada_tester.c test/hada_tester_n2p.c -I$CPATH

ls bin/