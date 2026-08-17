#!/bin/bash

SCRIPT_DIR=$(dirname "$(realpath "$0")")

echo "SCRIPT DIR IS $SCRIPT_DIR"
cd $SCRIPT_DIR/..


comp="guix time-machine -C ../forge-channels.scm -- shell -m ../fwht/guixmanifests/fwht_fftw_manifest.scm -- gcc"


FORGE_ROOT=$PWD/..
source $FORGE_ROOT/tools/export_path.bash
HADI_FWHT_INSTALL=hadi-fwht-master

echo $PWD

export CPATH=$FORGE_ROOT:$FORGE_ROOT/fwht_utils:$FORGE_ROOT/fwht:$FORGE_ROOT/sketch:${EXT_ROOT}/${HADI_FWHT_INSTALL}/include:$GUIX_ENVIRONMENT/include
export LD_LIBRARY_PATH=$GUIX_ENVIRONMENT/lib:${EXT_ROOT}/${HADI_FWHT_INSTALL}/lib:$LD_LIBRARY_PATH
#RESOLVE_CPATH="-I$FORGE_ROOT -I$FORGE_ROOT/fwht_utils -I$FORGE_ROOT/fwht -I${EXT_ROOT}/${HADI_FWHT_INSTALL}/include -I$GUIX_ENVIRONMENT/include"

RESOLVE_CPATH_FORGE="-I$FORGE_ROOT -I$FORGE_ROOT/fwht_utils -I$FORGE_ROOT/fwht -I$FORGE_ROOT/sketch"
RESOLVE_CPATH_GUIX="-I${GUIX_ENVIRONMENT}/include"
RESOLVE_CPATH_HADI="-I${EXT_ROOT}/${HADI_FWHT_INSTALL}/include"
RESOLVE_CPATH="${RESOLVE_CPATH_FORGE} ${RESOLVE_CPATH_GUIX} ${RESOLVE_CPATH_HADI}"

RESOLVE_LD_GUIX="-L${GUIX_ENVIRONMENT}/lib -lopenblas -lfftw3 -lfftw3_omp -lpthread -lm"
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



###########################################################################################################################################
# Sketch 1D block test t2
cat " " > build.log
touch build.log
for SK_ALGO in SRHT_CFWHT SRHT_FFTW SRHT_HADI_FWHT SRHT_HADI_FWHT_OPENMP GAUSS
do
    for SK_TYPE in SKETCH_2D
    do
        for MEM_ALLOC in USE_MALLOC USE_POSIX_MEMALIGN
        do
          echo "Sketch 1D block nocomm test t2 Using sketch algo ${SK_ALGO} and sketch type ${SK_TYPE}"
          $comp $debug_option -DFFTW_WITH_OMP -DSKETCH_ALG=${SK_ALGO} -DSKETCH_TYPE=${SK_TYPE} -D${MEM_ALLOC}  -o bin/tester_sketch_1D_block_nocomm_${SK_ALGO}_${SK_TYPE}_${MEM_ALLOC}_t2 \
          $FORGE_ROOT/sketch/sketch.c \
          $FORGE_ROOT/fwht_utils/hada.c \
          $FORGE_ROOT/fwht/base_fwht.c \
          test/sketch_tester_sketch_1D_block_nocomm_t2.c \
          ${RESOLVE_LD} \
          ${RESOLVE_CPATH} \
          2>&1 | tee -a build.log
          bin/tester_sketch_1D_block_nocomm_${SK_ALGO}_${SK_TYPE}_${MEM_ALLOC}_t2 |& tee -a build.log
          mv gmon.out prof/gmon-tester_sketch_1D_block_nocomm_${SK_ALGO}_${SK_TYPE}_${MEM_ALLOC}_t2.out
          gprof bin/tester_sketch_1D_block_nocomm_${SK_ALGO}_${SK_TYPE}_${MEM_ALLOC}_t2 prof/gmon-tester_sketch_1D_block_nocomm_${SK_ALGO}_${SK_TYPE}_${MEM_ALLOC}_t2.out > prof/gprof-tester_sketch_1D_block_nocomm_${SK_ALGO}_${SK_TYPE}_${MEM_ALLOC}_t2.out
          #valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes bin/tester_sketch_1D_block_nocomm_${SK_ALGO}_${SK_TYPE}_${MEM_ALLOC}_t2 2>&1 |& tee -a valgrind.log
        done
    done
done