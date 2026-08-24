#!/bin/bash

SCRIPT_DIR=$(dirname "$(realpath "$0")")

echo "SCRIPT DIR IS $SCRIPT_DIR"
cd $SCRIPT_DIR/..
GUIX_FILES=$PWD/../guix_files

comp="guix time-machine -C ${GUIX_FILES}/forge-channels.scm -- shell -m ${GUIX_FILES}/fwht_manifest.scm --file=${GUIX_FILES}/fftw.scm --file=${GUIX_FILES}/fwht.scm -- gcc"

run_jube_sketch() {
    guix time-machine -C ${GUIX_FILES}/forge-channels.scm -- shell -m ${GUIX_FILES}/fwht_manifest.scm --file=${GUIX_FILES}/fftw.scm --file=${GUIX_FILES}/fwht.scm -- bash -c "jube run jubescripts/benchmark_sketch_block_nocomm.xml"
}
run_R_sketch() {
    guix time-machine -C ${GUIX_FILES}/forge-channels.scm -- shell -m ${GUIX_FILES}/R_manifest.scm -- bash -c '
        shopt -s nullglob
        for f in output/sketch_block_nocomm_*_benchmark.csv; do
            echo "Génération du graphe pour $f"
            Rscript rscripts/gen_bench_sketch_block_nocomm.R "$f"
        done
    '
}

FORGE_ROOT=$PWD/..

mkdir -p bin/ logs/ output/ gen/
rm -f bin/* logs/* output/* gen/*

run_jube_sketch && run_R_sketch

mv *.pdf *.png *.html gen/