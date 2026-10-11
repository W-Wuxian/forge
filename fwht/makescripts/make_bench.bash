#!/bin/bash

SCRIPT_DIR=$(dirname "$(realpath "$0")")

echo "SCRIPT DIR IS $SCRIPT_DIR"
cd $SCRIPT_DIR/..
GUIX_FILES=$PWD/../guix_files
FWHT_BUILD_TYPE=fwht-fork-cuda-SM52.scm

comp="guix time-machine -C ${GUIX_FILES}/forge-channels.scm -- shell -m ${GUIX_FILES}/fwht_manifest.scm --file=${GUIX_FILES}/fftw.scm --file=${GUIX_FILES}/${FWHT_BUILD_TYPE} -- gcc"

run_jube() {
    guix time-machine -C ${GUIX_FILES}/forge-channels.scm -- shell -m ${GUIX_FILES}/fwht_manifest.scm --file=${GUIX_FILES}/fftw.scm --file=${GUIX_FILES}/${FWHT_BUILD_TYPE} --  bash -c "jube run jubescripts/benchmark.xml"
}
run_R() {
    guix time-machine -C ${GUIX_FILES}/forge-channels.scm -- shell -m ${GUIX_FILES}/R_manifest.scm -- bash -c '
        shopt -s nullglob
        for f in output/rotatedata_mat_*_benchmark.csv; do
            echo "Génération du graphe pour $f"
            Rscript rscripts/gen_bench.R "$f"
        done
    '
}
run_jube_fwht() {
    guix time-machine -C ${GUIX_FILES}/forge-channels.scm -- shell -m ${GUIX_FILES}/fwht_manifest.scm --file=${GUIX_FILES}/fftw.scm --file=${GUIX_FILES}/${FWHT_BUILD_TYPE} -- bash -c "jube run jubescripts/benchmark_fwht.xml"
}
run_R_fwht() {
    guix time-machine -C ${GUIX_FILES}/forge-channels.scm -- shell -m ${GUIX_FILES}/R_manifest.scm -- bash -c '
        shopt -s nullglob
        for f in output/fwht_mat_*_benchmark.csv; do
            echo "Génération du graphe pour $f"
            Rscript rscripts/gen_bench_fwht.R "$f"
        done
    '
}

FORGE_ROOT=$PWD/..

export CPATH=$FORGE_ROOT:$FORGE_ROOT/fwht_utils:$FORGE_ROOT/fwht:$GUIX_ENVIRONMENT/include
export LD_LIBRARY_PATH=$GUIX_ENVIRONMENT/lib:$LD_LIBRARY_PATH

export RESOLVE_CPATH_FORGE="-I$FORGE_ROOT -I$FORGE_ROOT/fwht_utils -I$FORGE_ROOT/fwht"
export RESOLVE_CPATH_GUIX="-I$GUIX_ENVIRONMENT/include"
export RESOLVE_CPATH="${RESOLVE_CPATH_FORGE} ${RESOLVE_CPATH_GUIX}"

export RESOLVE_LD_GUIX="-L$GUIX_ENVIRONMENT/lib -lopenblas -lfftw3 -lfftw3_omp -lpthread -lm -lfwht"
export RESOLVE_LD="${RESOLVE_LD_GUIX}"

mkdir -p bin/ logs/ output/ gen/
rm -f bin/* logs/* output/* gen/*

LOG_FILE=logs/bench.log
BENCH_OUT_FILE=output/bench_base_rotatedata_mat.csv


run_jube && run_R
run_jube_fwht && run_R_fwht

mv *.pdf *.png *.html gen/