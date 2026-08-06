#!/bin/bash

SCRIPT_DIR=$(dirname "$(realpath "$0")")

echo "SCRIPT DIR IS $SCRIPT_DIR"
cd $SCRIPT_DIR/..

comp="guix time-machine -C ../forge-channels.scm -- shell -m guixmanifests/fwht_manifest.scm -- gcc"

HADI_FWHT_INSTALL=hadi-fwht-master
FFTW_EXT_INSTALL=fftw-seq-double
FFTW_EXT_VER=3.3.11
EXT_ROOT=/mnt/e/WSL-WORK/forge/ext
FORGE_ROOT=$PWD/..

export CPATH=$FORGE_ROOT:$FORGE_ROOT/fwht_utils:$FORGE_ROOT/fwht:${EXT_ROOT}/${HADI_FWHT_INSTALL}/include:${EXT_ROOT}/${FFTW_EXT_INSTALL}/${FFTW_EXT_VER}/include:$GUIX_ENVIRONMENT/include
export LD_LIBRARY_PATH=$GUIX_ENVIRONMENT/lib:${EXT_ROOT}/${HADI_FWHT_INSTALL}/lib:${EXT_ROOT}/${FFTW_EXT_INSTALL}/${FFTW_EXT_VER}/lib:$LD_LIBRARY_PATH
#RESOLVE_CPATH="-I$FORGE_ROOT -I$FORGE_ROOT/fwht_utils -I$FORGE_ROOT/fwht -I${EXT_ROOT}/${HADI_FWHT_INSTALL}/include -I${EXT_ROOT}/${FFTW_EXT_INSTALL}/${FFTW_EXT_VER}/include"

RESOLVE_CPATH_FORGE="-I$FORGE_ROOT -I$FORGE_ROOT/fwht_utils -I$FORGE_ROOT/fwht"
RESOLVE_CPATH_GUIX="-I$GUIX_ENVIRONMENT/include"
RESOLVE_CPATH_HADI="-I${EXT_ROOT}/${HADI_FWHT_INSTALL}/include"
RESOLVE_CPATH_FFTW="-I${EXT_ROOT}/${FFTW_EXT_INSTALL}/${FFTW_EXT_VER}/include"
RESOLVE_CPATH="${RESOLVE_CPATH_FORGE} ${RESOLVE_CPATH_GUIX} ${RESOLVE_CPATH_HADI} ${RESOLVE_CPATH_FFTW}"

RESOLVE_LD_GUIX="-L$GUIX_ENVIRONMENT/lib -lopenblas -lpthread -lm"
RESOLVE_LD_HADI="-L${EXT_ROOT}/${HADI_FWHT_INSTALL}/lib -lfwht"
RESOLVE_LD_FFTW="-L${EXT_ROOT}/${FFTW_EXT_INSTALL}/${FFTW_EXT_VER}/lib -lfftw3"
RESOLVE_LD="${RESOLVE_LD_GUIX} ${RESOLVE_LD_HADI} ${RESOLVE_LD_FFTW}"

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
${RESOLVE_LD} \
${RESOLVE_CPATH} \
2>&1 | tee build.log
bin/showcase_fwht |& tee -a build.log
mv gmon.out prof/gmon-showcase_fwht.out
gprof bin/showcase_fwht prof/gmon-showcase_fwht.out > prof/grof-showcase_fwht.out


$comp $debug_option -o bin/test_base_dummy_fwht \
$FORGE_ROOT/fwht_utils/hada.c \
base_fwht.c test/fwht_tester.c test/fwht_tester_base_dummy_fwht.c \
${RESOLVE_LD} \
${RESOLVE_CPATH} \
2>&1 | tee -a build.log
bin/test_base_dummy_fwht |& tee -a build.log
mv gmon.out prof/gmon-test_base_dummy_fwht.out
gprof bin/test_base_dummy_fwht prof/gmon-test_base_dummy_fwht.out > prof/gprof-test_base_dummy_fwht.out


$comp $debug_option -o bin/test_base_rotatedata_mat \
$FORGE_ROOT/fwht_utils/hada.c \
base_fwht.c test/fwht_tester.c test/fwht_tester_rotatedata_mat.c \
${RESOLVE_LD} \
${RESOLVE_CPATH} \
2>&1 | tee -a build.log
bin/test_base_rotatedata_mat |& tee -a build.log
mv gmon.out prof/gmon-test_base_rotatedata_mat.out
gprof bin/test_base_rotatedata_mat prof/gmon-test_base_rotatedata_mat.out > prof/gprof-test_base_rotatedata_mat.out

$comp $debug_option -o bin/test_cblas_dgemm \
test/test_cblas_dgemm.c \
${RESOLVE_LD} \
${RESOLVE_CPATH} \
2>&1 | tee -a build.log
bin/test_cblas_dgemm |& tee -a build.log
mv gmon.out prof/gmon-test_cblas_dgemm.out
gprof bin/test_cblas_dgemm prof/gmon-test_cblas_dgemm.out > prof/gprof-test_cblas_dgemm.out

for li in bin/*
do
  ldd $li
done