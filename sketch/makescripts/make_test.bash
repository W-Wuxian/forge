#!/bin/bash

SCRIPT_DIR=$(dirname "$(realpath "$0")")

echo "SCRIPT DIR IS $SCRIPT_DIR"
cd $SCRIPT_DIR/..


comp="guix time-machine -C ../forge-channels.scm -- shell -m ../fwht/guixmanifests/fwht_fftw_manifest.scm -- gcc"


HADI_FWHT_INSTALL=hadi-fwht-master
EXT_ROOT=/home/vlederer/Bureau/DEV/ext
FORGE_ROOT=$PWD/..

echo $PWD

export CPATH=$FORGE_ROOT:$FORGE_ROOT/fwht_utils:$FORGE_ROOT/fwht:$FORGE_ROOT/sketch:${EXT_ROOT}/${HADI_FWHT_INSTALL}/include:$GUIX_ENVIRONMENT/include
export LD_LIBRARY_PATH=$GUIX_ENVIRONMENT/lib:${EXT_ROOT}/${HADI_FWHT_INSTALL}/lib:$LD_LIBRARY_PATH
#RESOLVE_CPATH="-I$FORGE_ROOT -I$FORGE_ROOT/fwht_utils -I$FORGE_ROOT/fwht -I${EXT_ROOT}/${HADI_FWHT_INSTALL}/include -I$GUIX_ENVIRONMENT/include"

RESOLVE_CPATH_FORGE="-I$FORGE_ROOT -I$FORGE_ROOT/fwht_utils -I$FORGE_ROOT/fwht -I$FORGE_ROOT/sketch"
RESOLVE_CPATH_GUIX="-I$GUIX_ENVIRONMENT/include"
RESOLVE_CPATH_HADI="-I${EXT_ROOT}/${HADI_FWHT_INSTALL}/include"
RESOLVE_CPATH="${RESOLVE_CPATH_FORGE} ${RESOLVE_CPATH_GUIX} ${RESOLVE_CPATH_HADI}"

RESOLVE_LD_GUIX="-L$GUIX_ENVIRONMENT/lib -lopenblas -lfftw3 -lpthread -lm"
RESOLVE_LD_HADI="-L${EXT_ROOT}/${HADI_FWHT_INSTALL}/lib -lfwht"
RESOLVE_LD="${RESOLVE_LD_GUIX} ${RESOLVE_LD_HADI}"

mkdir -p bin/
rm -f bin/*

mkdir -p prof/

#-DNDEBUG \
#debug_option="-O3 -march=native -g3 -pg -Wall -fopenmp -ftree-vectorize -fdump-tree-vect-details"
#debug_option="-O0 -g3 -pg -Wall -Wno-unknown-pragmas -DNOPRINTMAT"
debug_option="-O0 -g3 -pg -fopenmp -Wall -Wno-unknown-pragmas -DPRINTMAT"
#debug_option="-O3 -g3 -pg -Wall -fopenmp -ftree-vectorize -fdump-tree-vect-details -DPRINTMAT"
#debug_option="-O3 -march=native -Wall -fopenmp -fopt-info-vec-all=vec.report -DNOPRINTMAT"
#debug_option="-O3 -march=native -Wall -fopenmp -fopt-info-vec-optimized=vec_success.report -DPRINTMAT"

# showcase fwht
$comp $debug_option -o bin/tester_create_random_permutation \
$FORGE_ROOT/sketch/sketch.c \
$FORGE_ROOT/fwht_utils/hada.c \
$FORGE_ROOT/fwht/base_fwht.c \
test/sketch_tester_create_random_permutation.c \
${RESOLVE_LD} \
${RESOLVE_CPATH} \
2>&1 | tee build.log
bin/tester_create_random_permutation |& tee -a build.log
mv gmon.out prof/gmon-tester_create_random_permutation.out
gprof bin/tester_create_random_permutation prof/gmon-tester_create_random_permutation.out > prof/grof-tester_create_random_permutation.out


$comp $debug_option -o bin/tester_sketch_1D \
$FORGE_ROOT/sketch/sketch.c \
$FORGE_ROOT/fwht_utils/hada.c \
$FORGE_ROOT/fwht/base_fwht.c \
test/sketch_tester_sketch_1D.c \
${RESOLVE_LD} \
${RESOLVE_CPATH} \
2>&1 | tee build.log
bin/tester_sketch_1D |& tee -a build.log
mv gmon.out prof/gmon-tester_sketch_1D.out
gprof bin/tester_sketch_1D prof/gmon-tester_sketch_1D.out > prof/grof-tester_sketch_1D.out