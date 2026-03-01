#!/bin/bash

FORGE_ROOT=$HOME/Work/github/Forge
export CPATH=$FORGE_ROOT:$FORGE_ROOT/fwht_utils

gcc -O2 -Wall -o run hada.c test/hada_tester.c -DM=8 -I$CPATH
