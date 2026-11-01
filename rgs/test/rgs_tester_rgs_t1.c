/**
 * @file rgs_tester_rgs_t1.c
 * @author your name (you@domain.com)
 * @brief ftesting rgs
 * @version 0.1
 * @date 2026-08-22
 *
 * @copyright Copyright (c) 2026
 *
 */

#include "rgs.h"

#ifndef NROWS
#define NROWS 10
#endif

#ifndef NCOLS
#define NCOLS 5
#endif

#ifndef SKETCH_DIM
#define SKETCH_DIM 5
#endif

#ifndef WHICH_SKETCH_ALG
#define WHICH_SKETCH_ALG SRHT_FFTW
#endif

#ifndef WHICH_SKETCH_TYPE
#define WHICH_SKETCH_TYPE SKETCH_2D
#endif

#ifndef SKETCH_NUM_THREADS
#define SKETCH_NUM_THREADS 1
#endif

#ifndef SKETCH_NUM_GPUS
#define SKETCH_NUM_GPUS 1
#endif

int
main()
{
    int        ierr = 0;
    MPI_Comm   comm = NULL;
    int        rank = 0;
    int        size = 1;
    base_int_t rgs_iparam[IDX_RGS_IPARAM_LEN];
    rgs_iparam[IDX_NROWS_DATA_IN]      = NROWS;
    rgs_iparam[IDX_NCOLS_DATA_IN]      = NCOLS;
    rgs_iparam[IDX_SKETCH_DIM]         = SKETCH_DIM;
    rgs_iparam[IDX_NDL]                = 1;
    rgs_iparam[IDX_NDR]                = 1;
    rgs_iparam[IDX_SKETCH_ALG]         = WHICH_SKETCH_ALG;
    rgs_iparam[IDX_SKETCH_TYPE]        = WHICH_SKETCH_TYPE;
    rgs_iparam[IDX_SKETCH_NUM_THREADS] = SKETCH_NUM_THREADS;
    rgs_iparam[IDX_SKETCH_NUM_GPU]     = SKETCH_NUM_GPUS;

    rgs_iparam[IDX_USER_BLOCK_SIZE]                = 1;
    rgs_iparam[IDX_LSS_ALG]                        = RGS_CGS;
    rgs_iparam[IDX_NUMBER_REORTHO_LSS]             = 2;
    rgs_iparam[IDX_NUMBER_REORTHO_SKETCHED_CHOLQR] = 2;
    base_rgs_t rgs_data;
    base_set_rgs_data( &rgs_data, &rgs_iparam[0], rank, size, comm );
    base_display_mem( rgs_data.rgs_memory, "RGS Memory:" );
    base_free_rgs_data( &rgs_data );

    return ierr;
}