#!/bin/bash

FORGE_ROOT=$HOME/Work/github/Forge
export CPATH=$FORGE_ROOT:$FORGE_ROOT/fwht_utils

mkdir -p bin/
# showcase hada
gcc -O2 -Wall -o bin/showcase_hada hada.c test/hada_showcase.c -DM=16 -I$CPATH
#guix shell julia -- julia julia_hadamardize.jl

# Make test
# test trailing zero trz
gcc -O2 -Wall -o bin/test_trz hada.c test/hada_tester.c test/hada_tester_trz.c -I$CPATH
# test base_ilog2
gcc -O2 -Wall -o bin/test_ilog2 hada.c test/hada_tester.c test/hada_tester_ilog2.c -I$CPATH
# test base_ispow2
gcc -O2 -Wall -o bin/test_ispow2 hada.c test/hada_tester.c test/hada_tester_ispow2.c -I$CPATH
# test base_n2p
gcc -O2 -Wall -o bin/test_n2p hada.c test/hada_tester.c test/hada_tester_n2p.c -I$CPATH