#!/bin/bash

SCRIPT_DIR=$(dirname "$(realpath "$0")")

echo "SCRIPT DIR IS $SCRIPT_DIR"
cd $SCRIPT_DIR

if command -v guix >/dev/null 2>&1; then
    echo "Guix is installed."
    comp="guix shell gcc-toolchain@14.2.0 openblas -- gcc "
else
    echo "Guix is not installed."
    comp=gcc
fi

FORGE_ROOT=$PWD/..
export CPATH=$FORGE_ROOT:$FORGE_ROOT/fwht_utils:$FORGE_ROOT/fwht
RESOLVE_CPATH="-I$FORGE_ROOT -I$FORGE_ROOT/fwht_utils -I$FORGE_ROOT/fwht"
echo $CPATH

mkdir -p bin/
rm -f bin/*

$comp -O2 -Wall -o bin/bench_base_rotatedata_mat \
$FORGE_ROOT/fwht_utils/hada.c \
fwht.c bench/fwht_bench_rotatedata_mat.c \
-L$GUIX_ENVIRONMENT/lib -lopenblas -lpthread \
-I$GUIX_ENVIRONMENT/include \
$RESOLVE_CPATH \
2>&1 | tee -a bench.log
bin/bench_base_rotatedata_mat |& tee -a bench.log
