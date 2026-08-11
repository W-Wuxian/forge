#!/bin/bash

SCRIPT_DIR=$(dirname "$(realpath "$0")")

echo "SCRIPT DIR IS $SCRIPT_DIR"
cd $SCRIPT_DIR

./make_test_ext.bash && ./make_bench_ext.bash

