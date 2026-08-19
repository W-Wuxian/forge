#!/bin/bash

SCRIPT_DIR=$(dirname "$(realpath "$0")")

echo "SCRIPT DIR IS $SCRIPT_DIR"
cd $SCRIPT_DIR/..


comp="guix time-machine -C ../forge-channels.scm -- shell -m guixmanifests/fwht_manifest.scm -- gcc"

# run_jube_rotatedata_R() {
#     guix time-machine -C ../forge-channels.scm -- shell -m guixmanifests/fwht_fftw_R_manifest.scm --  bash -c "jube run jubescripts/benchmark.xml"
# }
# run_jube_fwht_R() {
#     guix time-machine -C ../forge-channels.scm -- shell -m guixmanifests/fwht_fftw_R_manifest.scm -- bash -c "jube run jubescripts/benchmark_fwht.xml"
# }

run_jube() {
    guix time-machine -C ../forge-channels.scm -- shell -m guixmanifests/fwht_fftw_manifest.scm --  bash -c "jube run jubescripts/benchmark.xml"
}
run_R() {
    guix time-machine -C ../forge-channels.scm -- shell -m guixmanifests/fwht_R_manifest.scm -- bash -c '
        shopt -s nullglob
        for f in output/rotatedata_mat_*_benchmark.csv; do
            echo "Génération du graphe pour $f"
            Rscript rscripts/gen_bench.R "$f"
        done
    '
}
run_jube_fwht() {
    guix time-machine -C ../forge-channels.scm -- shell -m guixmanifests/fwht_fftw_manifest.scm -- bash -c "jube run jubescripts/benchmark_fwht.xml"
}
run_R_fwht() {
    guix time-machine -C ../forge-channels.scm -- shell -m guixmanifests/fwht_R_manifest.scm -- bash -c '
        shopt -s nullglob
        for f in output/fwht_mat_*_benchmark.csv; do
            echo "Génération du graphe pour $f"
            Rscript rscripts/gen_bench_fwht.R "$f"
        done
    '
}

FORGE_ROOT=$PWD/..
source $FORGE_ROOT/tools/export_path.bash

export CPATH=$FORGE_ROOT:$FORGE_ROOT/fwht_utils:$FORGE_ROOT/fwht:${EXT_ROOT}/${HADI_FWHT_INSTALL}/include:$GUIX_ENVIRONMENT/include
export LD_LIBRARY_PATH=$GUIX_ENVIRONMENT/lib:${EXT_ROOT}/${HADI_FWHT_INSTALL}/lib:$LD_LIBRARY_PATH

export RESOLVE_CPATH_FORGE="-I$FORGE_ROOT -I$FORGE_ROOT/fwht_utils -I$FORGE_ROOT/fwht"
export RESOLVE_CPATH_GUIX="-I$GUIX_ENVIRONMENT/include"
export RESOLVE_CPATH_HADI="-I${EXT_ROOT}/${HADI_FWHT_INSTALL}/include"
export RESOLVE_CPATH="${RESOLVE_CPATH_FORGE} ${RESOLVE_CPATH_GUIX} ${RESOLVE_CPATH_HADI}"

export RESOLVE_LD_GUIX="-L$GUIX_ENVIRONMENT/lib -lopenblas -lfftw3 -lfftw3_omp -lpthread -lm"
export RESOLVE_LD_HADI="-L${EXT_ROOT}/${HADI_FWHT_INSTALL}/lib -lfwht"
export RESOLVE_LD="${RESOLVE_LD_GUIX} ${RESOLVE_LD_HADI}"

mkdir -p bin/ logs/ output/ gen/
rm -f bin/* logs/* output/* gen/*

LOG_FILE=logs/bench.log
BENCH_OUT_FILE=output/bench_base_rotatedata_mat.csv

#run_jube_rotatedata_R
#run_jube && run_R
run_jube_fwht && run_R_fwht

mv *.pdf *.png *.html gen/

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