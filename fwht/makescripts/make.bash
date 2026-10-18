#!/bin/bash

SCRIPT_DIR=$(dirname "$(realpath "$0")")
echo "SCRIPT DIR IS $SCRIPT_DIR"
. "${SCRIPT_DIR}"/../../tools/export_path.bash

cd $SCRIPT_DIR

./make_test.bash && ./make_bench.bash