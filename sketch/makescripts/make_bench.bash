#!/bin/bash

SCRIPT_DIR=$(dirname "$(realpath "$0")")

echo "SCRIPT DIR IS $SCRIPT_DIR"
cd $SCRIPT_DIR/..
GUIX_FILES=$PWD/../guix_files
FWHT_BUILD_TYPE=fwht-fork-cuda-SM52.scm

comp="guix time-machine -C ${GUIX_FILES}/forge-channels.scm -- shell -m ${GUIX_FILES}/fwht_manifest.scm --file=${GUIX_FILES}/fftw.scm --file=${GUIX_FILES}/${FWHT_BUILD_TYPE} -- gcc"

run_jube_sketch() {
    guix time-machine -C ${GUIX_FILES}/forge-channels.scm -- shell -m ${GUIX_FILES}/fwht_manifest.scm --file=${GUIX_FILES}/fftw.scm --file=${GUIX_FILES}/${FWHT_BUILD_TYPE} -- bash -c "srun -c 24 --gres=gpu:1 -t 08:00:00 --mem=50G jube -vvv run jubescripts/benchmark_sketch_block_nocomm.xml"
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

export CPATH=$FORGE_ROOT:$FORGE_ROOT/fwht_utils:$FORGE_ROOT/fwht:$FORGE_ROOT/sketch:$GUIX_ENVIRONMENT/include
export LD_LIBRARY_PATH=$GUIX_ENVIRONMENT/lib:$LD_LIBRARY_PATH

export RESOLVE_CPATH_FORGE="-I$FORGE_ROOT -I$FORGE_ROOT/fwht_utils -I$FORGE_ROOT/fwht -I$FORGE_ROOT/sketch"
export RESOLVE_CPATH_GUIX="-I$GUIX_ENVIRONMENT/include"
export RESOLVE_CPATH="${RESOLVE_CPATH_FORGE} ${RESOLVE_CPATH_GUIX}"

export RESOLVE_LD_GUIX="-L$GUIX_ENVIRONMENT/lib -lopenblas -lfftw3 -lfftw3_omp -lpthread -lm -lfwht"
export RESOLVE_LD="${RESOLVE_LD_GUIX}"

#mkdir -p bin/ logs/ output/ gen/
#rm -f bin/* logs/* output/* gen/*

#run_jube_sketch && run_R_sketch
run_R_sketch
mv *.pdf *.png *.html gen/
