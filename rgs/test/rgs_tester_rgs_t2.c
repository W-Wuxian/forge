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
#include "gdsm.h"

#ifndef SKETCH_DIM
#define SKETCH_DIM 3
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
    base_int_t m = 0, n = 0, nz = 0, lda = 0;
    size_t     mat_capacity    = 0;
    double    *data_need_facto = NULL;
    // ../MMTX/e05r0000.mtx
    // ../MMTX/nos1.mtx
    BASE_MATCREATE( &data_need_facto, dinitmmiodense, &mat_capacity, &m, &n, &nz, &lda, "../MMTX/nos1.mtx", BASE_COLMAJOR );
    PRINT_COLMAJ_MAT( data_need_facto, base_imin( 10, m ), base_imin( 10, n ), "data_need_facto", m, base_i_ze, base_i_ze );
    rgs_iparam[IDX_NROWS_DATA_IN]      = m;
    rgs_iparam[IDX_NCOLS_DATA_IN]      = n;
    rgs_iparam[IDX_SKETCH_DIM]         = base_imax( n, SKETCH_DIM );
    rgs_iparam[IDX_NDL]                = 1;
    rgs_iparam[IDX_NDR]                = 1;
    rgs_iparam[IDX_SKETCH_ALG]         = WHICH_SKETCH_ALG;
    rgs_iparam[IDX_SKETCH_TYPE]        = WHICH_SKETCH_TYPE;
    rgs_iparam[IDX_SKETCH_NUM_THREADS] = SKETCH_NUM_THREADS;
    rgs_iparam[IDX_SKETCH_NUM_GPU]     = SKETCH_NUM_GPUS;

    rgs_iparam[IDX_USER_BLOCK_SIZE]                = n;
    rgs_iparam[IDX_LSS_ALG]                        = RGS_CGS;
    rgs_iparam[IDX_NUMBER_REORTHO_LSS]             = 4;
    rgs_iparam[IDX_NUMBER_REORTHO_SKETCHED_CHOLQR] = 4;
    printf( "WHICH_SKETCH_ALG %s\n", string_sketch_alg[WHICH_SKETCH_ALG] );

    base_rgs_t rgs_data;

    base_set_rgs_data( &rgs_data, &rgs_iparam[0], rank, size, comm );
    base_display_stats( rgs_data.rgs_memory, rgs_data.rgs_flops, "RGS Memory:" );
    base_display_stats( rgs_data.sketch_data.sketch_memory, rgs_data.sketch_data.sketch_flops, "SKETCH Memory:" );
    printf( "#############################################################\n" );
    for ( base_int_t i = 0; i < rgs_data.number_of_block; ++i ) {
        PRINT_MSGERR( "ite=", i );
        PRINT_MSGERR( "rgs user block size", rgs_data.user_block_size );
        PRINT_MSGERR( "rgs current block size", rgs_data.current_block_size );
        PRINT_MSGERR( "rgs number of block", rgs_data.number_of_block );
        PRINT_MSGERR( "rgs remaining block size", rgs_data.remaining_block_size );
        PRINT_MSGIERR( "rgs current rgs ite", rgs_data.current_rgs_ite );
        base_get_data_rgs( &rgs_data, i, data_need_facto );
        base_compute_rgs( &rgs_data );
    }
    PRINT_COLMAJ_MAT( data_need_facto, base_imin( 10, m ), base_imin( 10, n ), "data refacto:", m, base_i_ze, base_i_ze );
    base_rgs_assert_error( &rgs_data, n );
    base_free_rgs_data( &rgs_data );
    SPEFREE( data_need_facto );

    return ierr;
}