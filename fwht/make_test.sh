#!/bin/bash

SCRIPT_DIR=$(dirname "$(realpath "$0")")

echo "SCRIPT DIR IS $SCRIPT_DIR"
cd $SCRIPT_DIR

if command -v guix >/dev/null 2>&1; then
    echo "Guix is installed."
    comp="guix shell gcc-toolchain@14.2.0 openblas fftw -- gcc "
else
    echo "Guix is not installed."
    comp=gcc
fi

FORGE_ROOT=$PWD/..
#export CPATH=$FORGE_ROOT:$FORGE_ROOT/fwht_utils:$FORGE_ROOT/fwht
#export LD_LIBRARY_PATH=$GUIX_ENVIRONMENT/lib:$LD_LIBRARY_PATH
export CPATH=$FORGE_ROOT:$FORGE_ROOT/fwht_utils:$FORGE_ROOT/fwht:$FORGE_ROOT/ext/hadi-fwht-2.0.1/include
export LD_LIBRARY_PATH=$GUIX_ENVIRONMENT/lib:$FORGE_ROOT/ext/hadi-fwht-2.0.1/lib:$LD_LIBRARY_PATH
RESOLVE_CPATH="-I$FORGE_ROOT -I$FORGE_ROOT/fwht_utils -I$FORGE_ROOT/fwht -I$FORGE_ROOT/ext/hadi-fwht-2.0.1/include"
echo $CPATH

mkdir -p bin/
rm -f bin/*

mkdir -p prof/

#-DNDEBUG \
#debug_option="-O3 -march=native -g3 -pg -Wall -fopenmp -ftree-vectorize -fdump-tree-vect-details"
#debug_option="-O0 -g3 -pg -Wall -Wno-unknown-pragmas -DNOPRINTMAT"
debug_option="-O0 -g3 -pg -Wall -Wno-unknown-pragmas -DPRINTMAT"
#debug_option="-O3 -g3 -pg -Wall -fopenmp -ftree-vectorize -fdump-tree-vect-details -DPRINTMAT"
#debug_option="-O3 -march=native -Wall -fopenmp -fopt-info-vec-all=vec.report -DNOPRINTMAT"
#debug_option="-O3 -march=native -Wall -fopenmp -fopt-info-vec-optimized=vec_success.report -DPRINTMAT"

# showcase fwht
$comp $debug_option -o bin/showcase_fwht \
$FORGE_ROOT/fwht_utils/hada.c \
base_fwht.c test/fwht_showcase.c \
-DFWHT_SHOWCASE_COL_M=16 \
-I$GUIX_ENVIRONMENT/include \
-L$GUIX_ENVIRONMENT/lib -lopenblas -lfftw3 -lpthread -lm \
-L/mnt/e/WSL-WORK/forge/ext/hadi-fwht-2.0.1/lib -lfwht \
$RESOLVE_CPATH \
2>&1 | tee build.log
bin/showcase_fwht |& tee -a build.log
mv gmon.out prof/gmon-showcase_fwht.out
gprof bin/showcase_fwht prof/gmon-showcase_fwht.out > prof/grof-showcase_fwht.out


$comp $debug_option -o bin/test_base_dummy_fwht \
$FORGE_ROOT/fwht_utils/hada.c \
base_fwht.c test/fwht_tester.c test/fwht_tester_base_dummy_fwht.c \
-L$GUIX_ENVIRONMENT/lib -lopenblas -lfftw3 -lpthread -lm \
-L/mnt/e/WSL-WORK/forge/ext/hadi-fwht-2.0.1/lib -lfwht \
-I$GUIX_ENVIRONMENT/include \
$RESOLVE_CPATH \
2>&1 | tee -a build.log
bin/test_base_dummy_fwht |& tee -a build.log
mv gmon.out prof/gmon-test_base_dummy_fwht.out
gprof bin/test_base_dummy_fwht prof/gmon-test_base_dummy_fwht.out > prof/gprof-test_base_dummy_fwht.out


$comp $debug_option -o bin/test_base_rotatedata_mat \
$FORGE_ROOT/fwht_utils/hada.c \
base_fwht.c test/fwht_tester.c test/fwht_tester_rotatedata_mat.c \
-L$GUIX_ENVIRONMENT/lib -lopenblas -lfftw3 -lpthread -lm \
-L/mnt/e/WSL-WORK/forge/ext/hadi-fwht-2.0.1/lib -lfwht \
-I$GUIX_ENVIRONMENT/include \
$RESOLVE_CPATH \
2>&1 | tee -a build.log
bin/test_base_rotatedata_mat |& tee -a build.log
mv gmon.out prof/gmon-test_base_rotatedata_mat.out
gprof bin/test_base_rotatedata_mat prof/gmon-test_base_rotatedata_mat.out > prof/gprof-test_base_rotatedata_mat.out

$comp $debug_option -o bin/test_cblas_dgemm \
test/test_cblas_dgemm.c \
-L$GUIX_ENVIRONMENT/lib -lopenblas -lfftw3 -lpthread -lm \
-I$GUIX_ENVIRONMENT/include \
2>&1 | tee -a build.log
bin/test_cblas_dgemm |& tee -a build.log
mv gmon.out prof/gmon-test_cblas_dgemm.out
gprof bin/test_cblas_dgemm prof/gmon-test_cblas_dgemm.out > prof/gprof-test_cblas_dgemm.out