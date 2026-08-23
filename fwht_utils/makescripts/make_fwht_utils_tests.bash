#!/bin/bash

SCRIPT_DIR=$(dirname "$(realpath "$0")")

echo "SCRIPT DIR IS $SCRIPT_DIR"
cd $SCRIPT_DIR/..
GUIX_FILES=$PWD/../guix_files

comp="guix time-machine -C ${GUIX_FILES}/forge-channels.scm -- shell -m ${GUIX_FILES}/fwht_manifest.scm --file=${GUIX_FILES}/fftw.scm --file=${GUIX_FILES}/fwht-2.0.1.scm -- gcc"


FORGE_ROOT=$PWD/..
source $FORGE_ROOT/tools/export_path.bash

RESOLVE_CPATH_FORGE="-I${FORGE_ROOT} -I${FORGE_ROOT}/fwht_utils"

mkdir -p bin/
rm -f bin/*

# showcase hada
$comp -DHADA_SHOWCASE_ROW_M=16 -O2 -Wall ${RESOLVE_CPATH_FORGE} -I${GUIX_ENVIRONMENT}/include -o bin/showcase_hada hada.c test/hada_showcase.c 2>&1 | tee bin/tests-logs.log
echo "showcase hada C" 2>&1 | tee -a bin/tests-logs.log
bin/showcase_hada |& tee -a bin/tests-logs.log
#echo "showcase hada Julia" 2>&1 | tee -a bin/tests-logs.log
#$comp_julia test/julia_hadamardize.jl 2>&1 | tee -a bin/tests-logs.log

# Make test
# test trailing zero trz
$comp -O2 -Wall -o bin/test_trz hada.c test/hada_tester.c test/hada_tester_trz.c ${RESOLVE_CPATH_FORGE} -I${GUIX_ENVIRONMENT}/include 2>&1 | tee -a bin/tests-logs.log
bin/test_trz |& tee -a bin/tests-logs.log
# test base_ilog2
$comp -O2 -Wall -o bin/test_ilog2 hada.c test/hada_tester.c test/hada_tester_ilog2.c ${RESOLVE_CPATH_FORGE} -I${GUIX_ENVIRONMENT}/include
bin/test_ilog2 |& tee -a bin/tests-logs.log
# test base_ispow2
$comp -O2 -Wall -o bin/test_ispow2 hada.c test/hada_tester.c test/hada_tester_ispow2.c ${RESOLVE_CPATH_FORGE} -I${GUIX_ENVIRONMENT}/include
bin/test_ispow2 |& tee -a bin/tests-logs.log
# test base_n2p
$comp -O2 -Wall -o bin/test_n2p hada.c test/hada_tester.c test/hada_tester_n2p.c ${RESOLVE_CPATH_FORGE} -I${GUIX_ENVIRONMENT}/include
bin/test_n2p |& tee -a bin/tests-logs.log