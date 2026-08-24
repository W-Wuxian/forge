#!/bin/bash

SCRIPT_DIR=$(dirname "$(realpath "$0")")

echo "SCRIPT DIR IS $SCRIPT_DIR"
cd $SCRIPT_DIR/..
GUIX_FILES=$PWD/../guix_files

comp="guix time-machine -C ${GUIX_FILES}/forge-channels.scm -- shell -m ${GUIX_FILES}/fwht_manifest.scm --file=${GUIX_FILES}/fftw.scm --file=${GUIX_FILES}/fwht.scm -- gcc"

run_jube() {
    guix time-machine -C ${GUIX_FILES}/forge-channels.scm -- shell -m ${GUIX_FILES}/fwht_manifest.scm --file=${GUIX_FILES}/fftw.scm --file=${GUIX_FILES}/fwht.scm --  bash -c "jube run jubescripts/benchmark.xml"
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
    guix time-machine -C ${GUIX_FILES}/forge-channels.scm -- shell -m ${GUIX_FILES}/fwht_manifest.scm --file=${GUIX_FILES}/fftw.scm --file=${GUIX_FILES}/fwht.scm -- bash -c "jube run jubescripts/benchmark_fwht.xml"
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

mkdir -p bin/ logs/ output/ gen/
rm -f bin/* logs/* output/* gen/*

LOG_FILE=logs/bench.log
BENCH_OUT_FILE=output/bench_base_rotatedata_mat.csv


run_jube && run_R
run_jube_fwht && run_R_fwht

mv *.pdf *.png *.html gen/