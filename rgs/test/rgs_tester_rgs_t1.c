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
#define NROWS 8
#endif

#ifndef NCOLS
#define NCOLS 2
#endif

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
    rgs_iparam[IDX_NROWS_DATA_IN]      = NROWS;
    rgs_iparam[IDX_NCOLS_DATA_IN]      = NCOLS;
    rgs_iparam[IDX_SKETCH_DIM]         = SKETCH_DIM;
    rgs_iparam[IDX_NDL]                = 1;
    rgs_iparam[IDX_NDR]                = 1;
    rgs_iparam[IDX_SKETCH_ALG]         = WHICH_SKETCH_ALG;
    rgs_iparam[IDX_SKETCH_TYPE]        = WHICH_SKETCH_TYPE;
    rgs_iparam[IDX_SKETCH_NUM_THREADS] = SKETCH_NUM_THREADS;
    rgs_iparam[IDX_SKETCH_NUM_GPU]     = SKETCH_NUM_GPUS;

    rgs_iparam[IDX_USER_BLOCK_SIZE]                = NCOLS;
    rgs_iparam[IDX_LSS_ALG]                        = RGS_CGS;
    rgs_iparam[IDX_NUMBER_REORTHO_LSS]             = 2;
    rgs_iparam[IDX_NUMBER_REORTHO_SKETCHED_CHOLQR] = 2;
    base_rgs_t rgs_data;
    double    *data_need_facto = NULL;
    SPEALLOC( data_need_facto, base_dalign, sizeof( double ) * NROWS * NCOLS );
    // dinitwrandrange( data_need_facto, NROWS * NCOLS, -10, 10, 1234567 );
    // PRINT_ROWMAJ_MAT( data_need_facto, NROWS, NCOLS, "data need refacto: dinitwrand user fixed seed" );
    // dinitwrandrange( data_need_facto, NROWS * NCOLS, -10, 10, 0 );
    // PRINT_ROWMAJ_MAT( data_need_facto, NROWS, NCOLS, "data need refacto: dinitwrand time bases seed" );
    // dinitlinear( data_need_facto, NROWS, NCOLS, BASE_ROWMAJOR );
    // PRINT_ROWMAJ_MAT( data_need_facto, NROWS, NCOLS, "data need refacto: dinitlinear BASE_ROWMAJOR" );
    dinitlinear( data_need_facto, NROWS, NCOLS, BASE_COLMAJOR );
    PRINT_COLMAJ_MAT( data_need_facto, NROWS, NCOLS, "data need refacto: dinitlinear BASE_COLMAJOR" );
    // dinitorthobasis_sincos( data_need_facto, NROWS, NCOLS, BASE_ROWMAJOR );
    // PRINT_ROWMAJ_MAT( data_need_facto, NROWS, NCOLS, "data need refacto: dinitorthobasis_sincos BASE_ROWMAJOR" );
    // dinitorthobasis_sincos( data_need_facto, NROWS, NCOLS, BASE_COLMAJOR );
    // PRINT_COLMAJ_MAT( data_need_facto, NROWS, NCOLS, "data need refacto: dinitorthobasis_sincos BASE_COLMAJOR" );
    base_set_rgs_data( &rgs_data, &rgs_iparam[0], rank, size, comm );
    base_display_stats( rgs_data.rgs_memory, rgs_data.rgs_flops, "RGS Memory:" );
    base_display_stats( rgs_data.sketch_data.sketch_memory, rgs_data.sketch_data.sketch_flops, "SKETCH Memory:" );
    PRINT_MSGERR( "rgs user block size", rgs_data.user_block_size );
    PRINT_MSGERR( "rgs current block size", rgs_data.current_block_size );
    PRINT_MSGERR( "rgs number of block", rgs_data.number_of_block );
    PRINT_MSGERR( "rgs remaining block size", rgs_data.remaining_block_size );
    PRINT_MSGERR( "rgs current rgs ite", rgs_data.current_rgs_ite );
    printf( "#############################################################\n" );
    for ( base_int_t i = 0; i < rgs_data.number_of_block; ++i ) {
        PRINT_MSGERR( "ite=", i );
        PRINT_MSGERR( "rgs user block size", rgs_data.user_block_size );
        PRINT_MSGERR( "rgs current block size", rgs_data.current_block_size );
        PRINT_MSGERR( "rgs number of block", rgs_data.number_of_block );
        PRINT_MSGERR( "rgs remaining block size", rgs_data.remaining_block_size );
        PRINT_MSGERR( "rgs current rgs ite", rgs_data.current_rgs_ite );
        base_get_data_rgs( &rgs_data, i, data_need_facto );
        base_compute_rgs( &rgs_data );
    }
    PRINT_COLMAJ_MAT( data_need_facto, NROWS, NCOLS, "data need refacto: Should be an orthogonal matrix now BASE_COLMAJOR" );
    base_rgs_assert_error( &rgs_data, NCOLS );
    base_free_rgs_data( &rgs_data );
    SPEFREE( data_need_facto );

    return ierr;
}