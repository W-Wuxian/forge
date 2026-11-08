#!/bin/bash

SCRIPT_DIR=$(dirname "$(realpath "$0")")
echo "SCRIPT DIR IS $SCRIPT_DIR"
. "${SCRIPT_DIR}"/../../tools/export_path.bash

cd $SCRIPT_DIR/..
echo "PWD $PWD"
GUIX_FILES=$PWD/../guix_files
#FWHT_BUILD_TYPE=fwht-fork-cuda-SM52.scm
echo "$GUIX_FILES"

comp="guix time-machine -C ${GUIX_FILES}/forge-channels.scm -- shell -m ${GUIX_FILES}/fwht_manifest.scm --file=${GUIX_FILES}/fftw.scm --file=${GUIX_FILES}/${FWHT_BUILD_TYPE} -- gcc"


FORGE_ROOT=$PWD/..
echo "FORGE_ROOT $FORGE_ROOT"
RESOLVE_CPATH_FORGE="-I$FORGE_ROOT -I$FORGE_ROOT/fwht_utils -I$FORGE_ROOT/fwht -I$FORGE_ROOT/sketch -I$FORGE_ROOT/rgs"

mkdir -p bin/
rm -f bin/*

mkdir -p prof/

#-DNDEBUG \
#debug_option="-O3 -march=native -g3 -pg -Wall -fopenmp -ftree-vectorize -fdump-tree-vect-details"
#debug_option="-O0 -g3 -pg -Wall -Wno-unknown-pragmas -DNOPRINTMAT"
#debug_option="-O0 -g3 -pg -fopenmp -Wall -Wno-unknown-pragmas -DPRINTMAT"
debug_option="-O0 -g3 -fopenmp -Wall -Wno-unknown-pragmas -DPRINTMAT"
#debug_option="-O3 -g3 -pg -Wall -fopenmp -ftree-vectorize -fdump-tree-vect-details -DPRINTMAT"
#debug_option="-O3 -march=native -Wall -fopenmp -fopt-info-vec-all=vec.report -DNOPRINTMAT"
#debug_option="-O3 -march=native -Wall -fopenmp -fopt-info-vec-optimized=vec_success.report -DPRINTMAT"
echo "" > build.log

###########################################################################################################################################
for SK_ALGO in SRHT_CFWHT
do
    for SK_TYPE in SKETCH_2D
    do
        echo "Sketch 1D block nocomm test t2 using sketch algo ${SK_ALGO} and sketch type ${SK_TYPE}"
        $comp $debug_option -DSKETCH_NUM_THREADS=1 -DSKETCH_NUM_GPUS=1 -DFFTW_WITH_OMP -DSKETCH_ALG=${SK_ALGO} -DSKETCH_TYPE=${SK_TYPE}  -o bin/tester_rgs_${SK_ALGO}_${SK_TYPE}_t1 \
        $FORGE_ROOT/rgs/rgs.c \
        $FORGE_ROOT/sketch/sketch.c \
        $FORGE_ROOT/fwht_utils/hada.c \
        $FORGE_ROOT/fwht/base_fwht.c \
        test/rgs_tester_rgs_t1.c \
        -L${GUIX_ENVIRONMENT}/lib -lopenblas -lfftw3 -lfftw3_omp -lpthread -lm -lfwht \
        ${RESOLVE_CPATH_FORGE} -I${GUIX_ENVIRONMENT}/include \
        2>&1 | tee -a build.log
        bin/tester_rgs_${SK_ALGO}_${SK_TYPE}_t1 |& tee -a build.log
        #mv gmon.out prof/gmon-tester_rgs_${SK_ALGO}_${SK_TYPE}_t1.out
        #gprof bin/tester_rgs_${SK_ALGO}_${SK_TYPE}_t1 prof/gmon-tester_rgs_${SK_ALGO}_${SK_TYPE}_t1.out > prof/gprof-tester_rgs_${SK_ALGO}_${SK_TYPE}_t1.out
        #valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes bin/tester_rgs_${SK_ALGO}_${SK_TYPE}_t1 2>&1 |& tee -a valgrind.log
    done
done

# for SK_ALGO in SRHT_CFWHT SRHT_FFTW SRHT_HADI_FWHT SRHT_HADI_FWHT_OPENMP
# do
#     for SK_TYPE in SKETCH_2D
#     do
#         echo "Sketch 1D block nocomm test t2 using sketch algo ${SK_ALGO} and sketch type ${SK_TYPE}"
#         $comp $debug_option -DSKETCH_NUM_THREADS=2 -DSKETCH_NUM_GPUS=1 -DFFTW_WITH_OMP -DSKETCH_ALG=${SK_ALGO} -DSKETCH_TYPE=${SK_TYPE}  -o bin/tester_rgs_${SK_ALGO}_${SK_TYPE}_t1 \
#         $FORGE_ROOT/sketch/rgs.c \
#         $FORGE_ROOT/sketch/sketch.c \
#         $FORGE_ROOT/fwht_utils/hada.c \
#         $FORGE_ROOT/fwht/base_fwht.c \
#         test/rgs_tester_rgs_t1.c \
#         -L${GUIX_ENVIRONMENT}/lib -lopenblas -lfftw3 -lfftw3_omp -lpthread -lm -lfwht \
#         ${RESOLVE_CPATH_FORGE} -I${GUIX_ENVIRONMENT}/include \
#         2>&1 | tee -a build.log
#         bin/tester_rgs_${SK_ALGO}_${SK_TYPE}_t1 |& tee -a build.log
#         #mv gmon.out prof/gmon-tester_rgs_${SK_ALGO}_${SK_TYPE}_t1.out
#         #gprof bin/tester_rgs_${SK_ALGO}_${SK_TYPE}_t1 prof/gmon-tester_rgs_${SK_ALGO}_${SK_TYPE}_t1.out > prof/gprof-tester_rgs_${SK_ALGO}_${SK_TYPE}_t1.out
#         #valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes bin/tester_rgs_${SK_ALGO}_${SK_TYPE}_t1 2>&1 |& tee -a valgrind.log
#     done
# done

# for SK_ALGO in SRHT_HADI_FWHT_GPU
# do
#     for SK_TYPE in SKETCH_2D
#     do
#         echo "Sketch 1D block nocomm test t2 using sketch algo ${SK_ALGO} and sketch type ${SK_TYPE}"
#         $comp $debug_option -DSKETCH_NUM_THREADS=2 -DSKETCH_NUM_GPUS=1 -DFFTW_WITH_OMP -DSKETCH_ALG=${SK_ALGO} -DSKETCH_TYPE=${SK_TYPE}  -o bin/tester_rgs_${SK_ALGO}_${SK_TYPE}_t1 \
#         $FORGE_ROOT/sketch/sketch.c \
#         $FORGE_ROOT/fwht_utils/hada.c \
#         $FORGE_ROOT/fwht/base_fwht.c \
#         test/rgs_tester_rgs_t1.c \
#         -L${GUIX_ENVIRONMENT}/lib -lopenblas -lfftw3 -lfftw3_omp -lpthread -lm -lfwht \
#         ${RESOLVE_CPATH_FORGE} -I${GUIX_ENVIRONMENT}/include \
#         2>&1 | tee -a build.log
#         LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libcuda.so.1 bin/tester_rgs_${SK_ALGO}_${SK_TYPE}_t1 |& tee -a build.log
#         #mv gmon.out prof/gmon-tester_rgs_${SK_ALGO}_${SK_TYPE}_t1.out
#         #gprof bin/tester_rgs_${SK_ALGO}_${SK_TYPE}_t1 prof/gmon-tester_rgs_${SK_ALGO}_${SK_TYPE}_t1.out > prof/gprof-tester_rgs_${SK_ALGO}_${SK_TYPE}_t1.out
#         #valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes bin/tester_rgs_${SK_ALGO}_${SK_TYPE}_t1 2>&1 |& tee -a valgrind.log
#     done
# done