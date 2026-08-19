#!/bin/bash

SCRIPT_DIR=$(dirname "$(realpath "$0")")

echo "SCRIPT DIR IS $SCRIPT_DIR"
cd $SCRIPT_DIR/..

comp="guix time-machine -C ../forge-channels.scm -- shell -m guixmanifests/fwht_manifest.scm -- gcc"

run_jube_sketch() {
    guix time-machine -C ../forge-channels.scm -- shell -m guixmanifests/fwht_fftw_manifest.scm -- bash -c "jube run jubescripts/benchmark_sketch_block_nocomm.xml"
}
run_R_sketch() {
    guix time-machine -C ../forge-channels.scm -- shell -m guixmanifests/R_manifest.scm -- bash -c '
        shopt -s nullglob
        for f in output/sketch_block_nocomm_*_benchmark.csv; do
            echo "Génération du graphe pour $f"
            Rscript rscripts/gen_bench_sketch_block_nocomm.R "$f"
        done
    '
}

FORGE_ROOT=$PWD/..
source $FORGE_ROOT/tools/export_path.bash

export CPATH=$FORGE_ROOT:$FORGE_ROOT/fwht_utils:$FORGE_ROOT/fwht:$FORGE_ROOT/sketch:${EXT_ROOT}/${HADI_FWHT_INSTALL}/include:$GUIX_ENVIRONMENT/include
export LD_LIBRARY_PATH=$GUIX_ENVIRONMENT/lib:${EXT_ROOT}/${HADI_FWHT_INSTALL}/lib:$LD_LIBRARY_PATH

export RESOLVE_CPATH_FORGE="-I$FORGE_ROOT -I$FORGE_ROOT/fwht_utils -I$FORGE_ROOT/fwht -I$FORGE_ROOT/sketch"
export RESOLVE_CPATH_GUIX="-I$GUIX_ENVIRONMENT/include"
export RESOLVE_CPATH_HADI="-I${EXT_ROOT}/${HADI_FWHT_INSTALL}/include"
export RESOLVE_CPATH="${RESOLVE_CPATH_FORGE} ${RESOLVE_CPATH_GUIX} ${RESOLVE_CPATH_HADI}"

export RESOLVE_LD_GUIX="-L$GUIX_ENVIRONMENT/lib -lopenblas -lfftw3 -lfftw3_omp -lpthread -lm"
export RESOLVE_LD_HADI="-L${EXT_ROOT}/${HADI_FWHT_INSTALL}/lib -lfwht"
export RESOLVE_LD="${RESOLVE_LD_GUIX} ${RESOLVE_LD_HADI}"

mkdir -p bin/ logs/ output/ gen/
rm -f bin/* logs/* output/* gen/*

run_jube_sketch && run_R_sketch

mv *.pdf *.png *.html gen/