#!/bin/bash

FORGE_ROOT=$HOME/Work/github/Forge
export CPATH=$FORGE_ROOT:$FORGE_ROOT/fwht_utils:$FORGE_ROOT/fwht
echo $CPATH

mkdir -p bin/
# showcase hada
gcc -O2 -Wall -o bin/showcase_fwht $FORGE_ROOT/fwht_utils/hada.c fwht.c test/fwht_tester.c -DM=8 -I$CPATH
#guix shell julia -- julia julia_hadamardize.jl
