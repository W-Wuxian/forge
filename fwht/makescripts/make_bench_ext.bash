#!/bin/bash

SCRIPT_DIR=$(dirname "$(realpath "$0")")

echo "SCRIPT DIR IS $SCRIPT_DIR"
cd $SCRIPT_DIR/..

source /home/vlederer/Bureau/DEV/ext/jube-2.7.1/bin/activate


comp=gcc
run_jube() {
    #guix time-machine -C ../forge-channels.scm -- shell -m fwht_manifest.scm --  bash -c "jube run benchmark.xml"
    jube run jubescripts/benchmark.xml
}
run_R() {
    guix time-machine -C ../forge-channels.scm -- shell -m guixmanifests/fwht_R_manifest.scm -- bash -c "Rscript rscripts/gen_bench.R output/rotatedata_mat_benchmark.csv"
}
run_jube_fwht() {
    #guix time-machine -C ../forge-channels.scm -- shell -m fwht_manifest.scm -- bash -c "jube run benchmark_fwht_ext.xml"
    jube run jubescripts/benchmark_fwht_ext.xml
}
run_R_fwht() {
    guix time-machine -C ../forge-channels.scm -- shell -m guixmanifests/fwht_R_manifest.scm -- bash -c "Rscript rscripts/gen_bench_fwht.R output/fwht_mat_benchmark.csv"
}

FORGE_ROOT=$PWD/..
source $FORGE_ROOT/tools/export_path.bash
HADI_FWHT_INSTALL=hadi-fwht-master
FFTW_EXT_VER=3.3.11
FFTW_EXT_INSTALL=fftw-$FFTW_EXT_VER
OPENBLAS_EXT_VER=0.3.34
OPENBLAS_EXT_INSTALL=openblas-$OPENBLAS_EXT_VER

export CPATH=$FORGE_ROOT:$FORGE_ROOT/fwht_utils:$FORGE_ROOT/fwht:${EXT_ROOT}/${HADI_FWHT_INSTALL}/include:${EXT_ROOT}/${FFTW_EXT_INSTALL}/include:${EXT_ROOT}/${OPENBLAS_EXT_INSTALL}/include
export LD_LIBRARY_PATH=${EXT_ROOT}/${HADI_FWHT_INSTALL}/lib:${EXT_ROOT}/${FFTW_EXT_INSTALL}/lib:${EXT_ROOT}/${OPENBLAS_EXT_INSTALL}/lib:$LD_LIBRARY_PATH
#RESOLVE_CPATH=="-I$FORGE_ROOT -I$FORGE_ROOT/fwht_utils -I$FORGE_ROOT/fwht -I${EXT_ROOT}/${HADI_FWHT_INSTALL}/include -I${EXT_ROOT}/${FFTW_EXT_INSTALL}/${FFTW_EXT_VER}/include"

export RESOLVE_CPATH_FORGE="-I$FORGE_ROOT -I$FORGE_ROOT/fwht_utils -I$FORGE_ROOT/fwht"
export RESOLVE_CPATH_HADI="-I${EXT_ROOT}/${HADI_FWHT_INSTALL}/include"
export RESOLVE_CPATH_FFTW="-I${EXT_ROOT}/${FFTW_EXT_INSTALL}/include"
export RESOLVE_CPATH_OPENBLAS="-I${EXT_ROOT}/${OPENBLAS_EXT_INSTALL}/include"
export RESOLVE_CPATH="${RESOLVE_CPATH_FORGE} ${RESOLVE_CPATH_OPENBLAS} ${RESOLVE_CPATH_HADI} ${RESOLVE_CPATH_FFTW}"

export RESOLVE_LD_HADI="-L${EXT_ROOT}/${HADI_FWHT_INSTALL}/lib -lfwht"
export RESOLVE_LD_FFTW="-L${EXT_ROOT}/${FFTW_EXT_INSTALL}/lib -lfftw3 -lfftw3_omp"
export RESOLVE_LD_OPENBLAS="-L${EXT_ROOT}/${OPENBLAS_EXT_INSTALL}/lib -lopenblas -lpthread -lm"
export RESOLVE_LD="${RESOLVE_LD_OPENBLAS} ${RESOLVE_LD_HADI} ${RESOLVE_LD_FFTW}"

# export RESOLVE_CPATH_FORGE="-I$FORGE_ROOT -I$FORGE_ROOT/fwht_utils -I$FORGE_ROOT/fwht"
# export RESOLVE_CPATH_GUIX="-I$GUIX_ENVIRONMENT/include"
# export RESOLVE_CPATH_HADI="-I${EXT_ROOT}/${HADI_FWHT_INSTALL}/include"
# export RESOLVE_CPATH_FFTW="-I${EXT_ROOT}/${FFTW_EXT_INSTALL}/${FFTW_EXT_VER}/include"
# export RESOLVE_CPATH="${RESOLVE_CPATH_FORGE} ${RESOLVE_CPATH_GUIX} ${RESOLVE_CPATH_HADI} ${RESOLVE_CPATH_FFTW}"

# export RESOLVE_LD_GUIX="-L$GUIX_ENVIRONMENT/lib -lopenblas -lpthread -lm"
# export RESOLVE_LD_HADI="-L${EXT_ROOT}/${HADI_FWHT_INSTALL}/lib -lfwht"
# export RESOLVE_LD_FFTW="-L${EXT_ROOT}/${FFTW_EXT_INSTALL}/${FFTW_EXT_VER}/lib -lfftw3 -lfftw3_omp"
# export RESOLVE_LD="${RESOLVE_LD_GUIX} ${RESOLVE_LD_HADI} ${RESOLVE_LD_FFTW}"

mkdir -p bin/ logs/ output/ gen/
rm -f bin/* logs/* output/* gen/*

LOG_FILE=logs/bench.log
BENCH_OUT_FILE=output/bench_base_rotatedata_mat.csv

run_jube && run_R
run_jube_fwht && run_R_fwht

mv *.pdf *.png *.html gen/

#run_jube && guix shell r r-tidyverse -- Rscript gen_bench.R output/rotatedata_mat_benchmark.csv
#run_jube_fwht && guix shell r r-tidyverse r-plotly -- Rscript gen_bench_fwht.R output/fwht_mat_benchmark.csv


# this one is a dummy one testing fixture ubench
# $comp -O2 -Wall -o bin/bench_base_fixture_rotatedata_mat \
# $FORGE_ROOT/fwht_utils/hada.c \
# base_fwht.c bench/fwht_bench_fixture_rotatedata_mat.c \
# -L$GUIX_ENVIRONMENT/lib -lopenblas -lpthread -lm \
# -I$GUIX_ENVIRONMENT/include \
# $RESOLVE_CPATH \
# 2>&1 | tee -a ${LOG_FILE}
# bin/bench_base_fixture_rotatedata_mat |& tee -a ${LOG_FILE}

# this one can be run as it, need manual bench case extension from source
# $comp -O2 -Wall -o bin/benchv2_base_rotatedata_mat \
# $FORGE_ROOT/fwht_utils/hada.c \
# base_fwht.c bench/fwht_benchv2_rotatedata_mat.c \
# -L$GUIX_ENVIRONMENT/lib -lopenblas -lpthread -lm \
# -I$GUIX_ENVIRONMENT/include \
# $RESOLVE_CPATH \
# 2>&1 | tee -a ${LOG_FILE}
# bin/benchv2_base_rotatedata_mat --output=${BENCH_OUT_FILE}
# #|& tee -a ${LOG_FILE}

# this one is used by jube cannot be run without it
# $comp -O2 -Wall -o bin/bench_base_rotatedata_mat \
# $FORGE_ROOT/fwht_utils/hada.c \
# base_fwht.c bench/fwht_bench_rotatedata_mat.c \
# -L$GUIX_ENVIRONMENT/lib -lopenblas -lpthread -lm \
# -I$GUIX_ENVIRONMENT/include \
# $RESOLVE_CPATH \
# 2>&1 | tee -a ${LOG_FILE}
# bin/bench_base_rotatedata_mat --output=${BENCH_OUT_FILE}

#echo "bin/bench_base_rotatedata_mat $FORGE_ROOT/fwht_utils/hada.c base_fwht.c bench/fwht_bench_rotatedata_mat.c -L$GUIX_ENVIRONMENT/lib -lopenblas -lpthread -lm -I$GUIX_ENVIRONMENT/include $RESOLVE_CPATH"