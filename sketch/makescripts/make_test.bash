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

RESOLVE_LD_GUIX="-L${GUIX_ENVIRONMENT}/lib -lopenblas -lfftw3 -lpthread -lm"
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

# Create Random Permutation
echo "Create Random Permutation"
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
#valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes bin/tester_create_random_permutation 2>&1 |& tee valgrind.log

###########################################################################################################################################
# Sketch 1D using sketch algo GAUSS
echo "Sketch 1D using sketch algo GAUSS"
$comp $debug_option -DWHICH_SKETCH_ALG=GAUSS -o bin/tester_sketch_1D_GAUSS \
$FORGE_ROOT/sketch/sketch.c \
$FORGE_ROOT/fwht_utils/hada.c \
$FORGE_ROOT/fwht/base_fwht.c \
test/sketch_tester_sketch_1D.c \
${RESOLVE_LD} \
${RESOLVE_CPATH} \
2>&1 | tee build.log
bin/tester_sketch_1D_GAUSS |& tee -a build.log
mv gmon.out prof/gmon-tester_sketch_1D_GAUSS.out
gprof bin/tester_sketch_1D_GAUSS prof/gmon-tester_sketch_1D_GAUSS.out > prof/grof-tester_sketch_1D_GAUSS.out
#valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes bin/tester_sketch_1D_GAUSS 2>&1 |& tee -a valgrind.log

# Sketch 1D using sketch algo SRHT_FFTW
echo "Sketch 1D using sketch algo SRHT_FFTW"
$comp $debug_option -DWHICH_SKETCH_ALG=SRHT_FFTW -o bin/tester_sketch_1D_SRHT_FFTW \
$FORGE_ROOT/sketch/sketch.c \
$FORGE_ROOT/fwht_utils/hada.c \
$FORGE_ROOT/fwht/base_fwht.c \
test/sketch_tester_sketch_1D.c \
${RESOLVE_LD} \
${RESOLVE_CPATH} \
2>&1 | tee build.log
bin/tester_sketch_1D_SRHT_FFTW |& tee -a build.log
mv gmon.out prof/gmon-tester_sketch_1D_SRHT_FFTW.out
gprof bin/tester_sketch_1D_SRHT_FFTW prof/gmon-tester_sketch_1D_SRHT_FFTW.out > prof/grof-tester_sketch_1D_SRHT_FFTW.out
#valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes bin/tester_sketch_1D_SRHT_FFTW 2>&1 |& tee -a valgrind.log

# Sketch 1D using sketch algo SRHT_HADI_FWHT
echo "Sketch 1D using sketch algo SRHT_HADI_FWHT"
$comp $debug_option -DWHICH_SKETCH_ALG=SRHT_HADI_FWHT -o bin/tester_sketch_1D_SRHT_HADI_FWHT \
$FORGE_ROOT/sketch/sketch.c \
$FORGE_ROOT/fwht_utils/hada.c \
$FORGE_ROOT/fwht/base_fwht.c \
test/sketch_tester_sketch_1D.c \
${RESOLVE_LD} \
${RESOLVE_CPATH} \
2>&1 | tee build.log
bin/tester_sketch_1D_SRHT_HADI_FWHT |& tee -a build.log
mv gmon.out prof/gmon-tester_sketch_1D_SRHT_HADI_FWHT.out
gprof bin/tester_sketch_1D_SRHT_HADI_FWHT prof/gmon-tester_sketch_1D_SRHT_HADI_FWHT.out > prof/grof-tester_sketch_1D_SRHT_HADI_FWHT.out
#valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes bin/tester_sketch_1D_SRHT_HADI_FWHT 2>&1 |& tee -a valgrind.log

# Sketch 1D using sketch algo SRHT_CFWHT
echo "Sketch 1D using sketch algo SRHT_CFWHT"
$comp $debug_option -DWHICH_SKETCH_ALG=SRHT_CFWHT -o bin/tester_sketch_1D_SRHT_CFWHT \
$FORGE_ROOT/sketch/sketch.c \
$FORGE_ROOT/fwht_utils/hada.c \
$FORGE_ROOT/fwht/base_fwht.c \
test/sketch_tester_sketch_1D.c \
${RESOLVE_LD} \
${RESOLVE_CPATH} \
2>&1 | tee build.log
bin/tester_sketch_1D_SRHT_CFWHT |& tee -a build.log
mv gmon.out prof/gmon-tester_sketch_1D_SRHT_CFWHT.out
gprof bin/tester_sketch_1D_SRHT_CFWHT prof/gmon-tester_sketch_1D_SRHT_CFWHT.out > prof/grof-tester_sketch_1D_SRHT_CFWHT.out
#valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes bin/tester_sketch_1D_SRHT_CFWHT 2>&1 |& tee -a valgrind.log

###########################################################################################################################################
# Sketch 1D test t1 Using sketch algo GAUSS
echo "Sketch 1D test t1 Using sketch algo GAUSS"
$comp $debug_option -DWHICH_SKETCH_ALG=GAUSS -o bin/tester_sketch_1D_GAUSS_t1 \
$FORGE_ROOT/sketch/sketch.c \
$FORGE_ROOT/fwht_utils/hada.c \
$FORGE_ROOT/fwht/base_fwht.c \
test/sketch_tester_sketch_1D_t1.c \
${RESOLVE_LD} \
${RESOLVE_CPATH} \
2>&1 | tee build.log
bin/tester_sketch_1D_GAUSS_t1 |& tee build.log
mv gmon.out prof/gmon-tester_sketch_1D_GAUSS_t1.out
gprof bin/tester_sketch_1D_GAUSS_t1 prof/gmon-tester_sketch_1D_GAUSS_t1.out > prof/grof-tester_sketch_1D_GAUSS_t1.out
#valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes bin/tester_sketch_1D_GAUSS_t1 2>&1 |& tee -a valgrind.log

# Sketch 1D test t1 Using sketch algo SRHT_FFTW
echo "Sketch 1D test t1 Using sketch algo SRHT_FFTW"
$comp $debug_option -DWHICH_SKETCH_ALG=SRHT_FFTW -o bin/tester_sketch_1D_SRHT_FFTW_t1 \
$FORGE_ROOT/sketch/sketch.c \
$FORGE_ROOT/fwht_utils/hada.c \
$FORGE_ROOT/fwht/base_fwht.c \
test/sketch_tester_sketch_1D_t1.c \
${RESOLVE_LD} \
${RESOLVE_CPATH} \
2>&1 | tee build.log
bin/tester_sketch_1D_SRHT_FFTW_t1 |& tee -a build.log
mv gmon.out prof/gmon-tester_sketch_1D_SRHT_FFTW_t1.out
gprof bin/tester_sketch_1D_SRHT_FFTW_t1 prof/gmon-tester_sketch_1D_SRHT_FFTW_t1.out > prof/grof-tester_sketch_1D_SRHT_FFTW_t1.out
#valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes bin/tester_sketch_1D_SRHT_FFTW_t1 2>&1 |& tee -a valgrind.log

# Sketch 1D test t1 Using sketch algo SRHT_HADI_FWHT
echo "Sketch 1D test t1 Using sketch algo SRHT_HADI_FWHT"
$comp $debug_option -DWHICH_SKETCH_ALG=SRHT_HADI_FWHT -o bin/tester_sketch_1D_SRHT_HADI_FWHT_t1 \
$FORGE_ROOT/sketch/sketch.c \
$FORGE_ROOT/fwht_utils/hada.c \
$FORGE_ROOT/fwht/base_fwht.c \
test/sketch_tester_sketch_1D_t1.c \
${RESOLVE_LD} \
${RESOLVE_CPATH} \
2>&1 | tee build.log
bin/tester_sketch_1D_SRHT_HADI_FWHT_t1 |& tee -a build.log
mv gmon.out prof/gmon-tester_sketch_1D_SRHT_HADI_FWHT_t1.out
gprof bin/tester_sketch_1D_SRHT_HADI_FWHT_t1 prof/gmon-tester_sketch_1D_SRHT_HADI_FWHT_t1.out > prof/grof-tester_sketch_1D_SRHT_HADI_FWHT_t1.out
#valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes bin/tester_sketch_1D_SRHT_HADI_FWHT_t1 2>&1 |& tee -a valgrind.log

# Sketch 1D test t1 Using sketch algo SRHT_CFWHT
echo "Sketch 1D test t1 Using sketch algo SRHT_CFWHT"
$comp $debug_option -DWHICH_SKETCH_ALG=SRHT_CFWHT -o bin/tester_sketch_1D_SRHT_CFWHT_t1 \
$FORGE_ROOT/sketch/sketch.c \
$FORGE_ROOT/fwht_utils/hada.c \
$FORGE_ROOT/fwht/base_fwht.c \
test/sketch_tester_sketch_1D_t1.c \
${RESOLVE_LD} \
${RESOLVE_CPATH} \
2>&1 | tee build.log
bin/tester_sketch_1D_SRHT_CFWHT_t1 |& tee -a build.log
mv gmon.out prof/gmon-tester_sketch_1D_SRHT_CFWHT_t1.out
gprof bin/tester_sketch_1D_SRHT_CFWHT_t1 prof/gmon-tester_sketch_1D_SRHT_CFWHT_t1.out > prof/grof-tester_sketch_1D_SRHT_CFWHT_t1.out
#valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes bin/tester_sketch_1D_SRHT_CFWHT_t1 2>&1 |& tee -a valgrind.log

###########################################################################################################################################
# Sketch 1D mat test t1 Using sketch algo GAUSS
echo "Sketch 1D mat test t1 Using sketch algo GAUSS"
$comp $debug_option -DWHICH_SKETCH_ALG=GAUSS -o bin/tester_sketch_1D_mat_GAUSS_t1 \
$FORGE_ROOT/sketch/sketch.c \
$FORGE_ROOT/fwht_utils/hada.c \
$FORGE_ROOT/fwht/base_fwht.c \
test/sketch_tester_sketch_1D_mat_t1.c \
${RESOLVE_LD} \
${RESOLVE_CPATH} \
2>&1 | tee build.log
bin/tester_sketch_1D_mat_GAUSS_t1 |& tee -a build.log
mv gmon.out prof/gmon-tester_sketch_1D_mat_GAUSS_t1.out
gprof bin/tester_sketch_1D_mat_GAUSS_t1 prof/gmon-tester_sketch_1D_mat_GAUSS_t1.out > prof/grof-tester_sketch_1D_mat_GAUSS_t1.out
#valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes bin/tester_sketch_1D_mat_GAUSS_t1 2>&1 |& tee -a valgrind.log

# Sketch 1D mat test t1 Using sketch algo SRHT_FFTW
echo "Sketch 1D mat test t1 Using sketch algo SRHT_FFTW"
$comp $debug_option -DWHICH_SKETCH_ALG=SRHT_FFTW -o bin/tester_sketch_1D_mat_SRHT_FFTW_t1 \
$FORGE_ROOT/sketch/sketch.c \
$FORGE_ROOT/fwht_utils/hada.c \
$FORGE_ROOT/fwht/base_fwht.c \
test/sketch_tester_sketch_1D_mat_t1.c \
${RESOLVE_LD} \
${RESOLVE_CPATH} \
2>&1 | tee build.log
bin/tester_sketch_1D_mat_SRHT_FFTW_t1 |& tee -a build.log
mv gmon.out prof/gmon-tester_sketch_1D_mat_SRHT_FFTW_t1.out
gprof bin/tester_sketch_1D_mat_SRHT_FFTW_t1 prof/gmon-tester_sketch_1D_mat_SRHT_FFTW_t1.out > prof/grof-tester_sketch_1D_mat_SRHT_FFTW_t1.out
#valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes bin/tester_sketch_1D_mat_SRHT_FFTW_t1 2>&1 |& tee -a valgrind.log

# Sketch 1D mat test t1 Using sketch algo SRHT_HADI_FWHT
echo "Sketch 1D mat test t1 Using sketch algo SRHT_HADI_FWHT"
$comp $debug_option -DWHICH_SKETCH_ALG=SRHT_HADI_FWHT -o bin/tester_sketch_1D_mat_SRHT_HADI_FWHT_t1 \
$FORGE_ROOT/sketch/sketch.c \
$FORGE_ROOT/fwht_utils/hada.c \
$FORGE_ROOT/fwht/base_fwht.c \
test/sketch_tester_sketch_1D_mat_t1.c \
${RESOLVE_LD} \
${RESOLVE_CPATH} \
2>&1 | tee build.log
bin/tester_sketch_1D_mat_SRHT_HADI_FWHT_t1 |& tee -a build.log
mv gmon.out prof/gmon-tester_sketch_1D_mat_SRHT_HADI_FWHT_t1.out
gprof bin/tester_sketch_1D_mat_SRHT_HADI_FWHT_t1 prof/gmon-tester_sketch_1D_mat_SRHT_HADI_FWHT_t1.out > prof/grof-tester_sketch_1D_mat_SRHT_HADI_FWHT_t1.out
#valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes bin/tester_sketch_1D_mat_SRHT_HADI_FWHT_t1 2>&1 |& tee -a valgrind.log

# Sketch 1D mat test t1 Using sketch algo SRHT_CFWHT
echo "Sketch 1D mat test t1 Using sketch algo SRHT_CFWHT"
$comp $debug_option -DWHICH_SKETCH_ALG=SRHT_CFWHT -o bin/tester_sketch_1D_mat_SRHT_CFWHT_t1 \
$FORGE_ROOT/sketch/sketch.c \
$FORGE_ROOT/fwht_utils/hada.c \
$FORGE_ROOT/fwht/base_fwht.c \
test/sketch_tester_sketch_1D_mat_t1.c \
${RESOLVE_LD} \
${RESOLVE_CPATH} \
2>&1 | tee build.log
bin/tester_sketch_1D_mat_SRHT_CFWHT_t1 |& tee -a build.log
mv gmon.out prof/gmon-tester_sketch_1D_mat_SRHT_CFWHT_t1.out
gprof bin/tester_sketch_1D_mat_SRHT_CFWHT_t1 prof/gmon-tester_sketch_1D_mat_SRHT_CFWHT_t1.out > prof/grof-tester_sketch_1D_mat_SRHT_CFWHT_t1.out
#valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes bin/tester_sketch_1D_mat_SRHT_CFWHT_t1 2>&1 |& tee -a valgrind.log