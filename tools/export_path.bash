#!/bin/bash

NVCAP0=$(nvidia-smi --query-gpu=compute_cap --format=csv,noheader 2>/dev/null)
NVCAP1="${NVCAP0//./}"
CUDA_ARCH="--with-cuda-arch=${NVCAP1}"
echo "NVIDIA compute cap is $NVCAP0"
echo "$CUDA_ARCH"
export GUIX_FILE_CUDA_ARCH="fwht-fork-cuda-SM${NVCAP1}.scm"
echo "${GUIX_FILE_CUDA_ARCH}"
if [ ! -f ${GUIX_FILE_CUDA_ARCH} ]
then
  cp ../guix_files/fwht-fork-cuda-SM52.scm ../guix_files/${GUIX_FILE_CUDA_ARCH}
  sed -i "s/--with-cuda-arch=52/--with-cuda-arch=${NVCAP1}/g" ../guix_files/${GUIX_FILE_CUDA_ARCH}
fi
export FWHT_BUILD_TYPE=${GUIX_FILE_CUDA_ARCH}

#export EXT_ROOT=/mnt/e/WSL-WORK/ext-libs
# export EXT_ROOT=/home/wuxian/Work/github/FORGE/ext-libs

# HADI_FWHT_INSTALL=hadi-fwht-omp

# FFTW_EXT_VER=3.3.11
# FFTW_EXT_INSTALL=fftw-$FFTW_EXT_VER
# OPENBLAS_EXT_VER=0.3.34
# OPENBLAS_EXT_INSTALL=openblas-$OPENBLAS_EXT_VER
