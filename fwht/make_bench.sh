#!/bin/bash

SCRIPT_DIR=$(dirname "$(realpath "$0")")

echo "SCRIPT DIR IS $SCRIPT_DIR"
cd $SCRIPT_DIR

if command -v guix >/dev/null 2>&1; then
    echo "Guix is installed."
    comp="guix shell gcc-toolchain@14.2.0 openblas jube -- gcc "
    run_jube() {
        guix shell --tune gcc-toolchain@14.2.0 openblas jube -- bash -c "jube run benchmark.xml"
    }
    run_jube_fwht() {
        guix shell --tune gcc-toolchain@14.2.0 openblas fftw jube -- bash -c "jube run benchmark_fwht.xml"
    }
else
    echo "Guix is not installed."
    comp=gcc
    run_jube() {
        jube run benchmark.xml
    }
fi

FORGE_ROOT=$PWD/..
export CPATH=$FORGE_ROOT:$FORGE_ROOT/fwht_utils:$FORGE_ROOT/fwht:$FORGE_ROOT/ext/hadi-fwht-2.0.1/include
export LD_LIBRARY_PATH=$GUIX_ENVIRONMENT/lib:$FORGE_ROOT/ext/hadi-fwht-2.0.1/lib:$LD_LIBRARY_PATH
RESOLVE_CPATH="-I$FORGE_ROOT -I$FORGE_ROOT/fwht_utils -I$FORGE_ROOT/fwht -I$FORGE_ROOT/ext/hadi-fwht-2.0.1/include"
echo $CPATH

mkdir -p bin/ logs/ output/
rm -f bin/* logs/* output/*

LOG_FILE=logs/bench.log
BENCH_OUT_FILE=output/bench_base_rotatedata_mat.csv

#run_jube && guix shell r r-tidyverse -- Rscript gen_bench.R output/rotatedata_mat_benchmark.csv
run_jube_fwht && guix shell r r-tidyverse r-plotly -- Rscript gen_bench_fwht.R output/fwht_mat_benchmark.csv
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