#include "fwht.h"

#ifndef FWHT_SHOWCASE_ROW_M
#define FWHT_SHOWCASE_ROW_M 16
#endif

#ifndef FWHT_SHOWCASE_COL_N
#define FWHT_SHOWCASE_COL_N 1
#endif

// ref solution https://en.wikipedia.org/wiki/Fast_Walsh%E2%80%93Hadamard_transform

int
main()
{
    base_int_t ierr        = 0;
    base_int_t ref_sol_dim = 8 * 1;  // FWHT_SHOWCASE_ROW_M = 8 FWHT_SHOWCASE_COL_N = 1

    double data[FWHT_SHOWCASE_ROW_M * FWHT_SHOWCASE_COL_N];
    for ( base_int_t i = 0; i < FWHT_SHOWCASE_ROW_M * FWHT_SHOWCASE_COL_N; i++ ) {
        data[i] = (double)0;
    }
    data[0] = 1.;
    data[1] = 0.;
    data[2] = 1.;
    data[3] = 0.;
    data[4] = 0.;
    data[5] = 1.;
    data[6] = 1.;
    data[7] = 0.;
    base_dummy_fwht( &data[0], ref_sol_dim );

    PRINT_ROWMAJ_MAT( &data[0], FWHT_SHOWCASE_ROW_M, FWHT_SHOWCASE_COL_N, "base_dummy_fwht :: Solution is:" );

    // Testing transpose:
    // base_int_t nr = 3;
    // base_int_t nc = 4;
    // double     A[nr * nc];       // assuming A is Col-Major
    // double     TransA[nc * nr];  // assuming TransA is Col-Major
    // for ( base_int_t i = 0; i < nc; ++i ) {
    //     for ( base_int_t j = 0; j < nr; ++j ) {
    //         A[j + ( i * nr )] = j + ( i * nr );
    //     }
    // }
    // printf( "A matrix Col-Major\n" );
    // for ( base_int_t i = 0; i < nr * nc; ++i ) {
    //     printf( "%f ", *A + i );
    // }
    // printf( "\n" );
    // printf( "A matrix Col-Major\n" );
    // for ( base_int_t i = 0; i < nr; ++i ) {
    //     for ( base_int_t j = 0; j < nc; ++j ) {
    //         printf( "%f ", *( A + i + ( j * nr ) ) );
    //     }
    //     printf( "\n" );
    // }

    // for ( base_int_t i = 0; i < nc; ++i ) {
    //     for ( base_int_t j = 0; j < nr; ++j ) {
    //         *( TransA + i + ( j * nc ) ) = *( A + j + ( i * nr ) );
    //     }
    // }
    // printf( "\n" );
    // printf( "TransA matrix Col-Major\n" );
    // for ( base_int_t i = 0; i < nr * nc; ++i ) {
    //     printf( "%f ", *TransA + i );
    // }
    // printf( "\n" );
    // printf( "TransA matrix Col-Major\n" );
    // for ( base_int_t i = 0; i < nc; ++i ) {
    //     for ( base_int_t j = 0; j < nr; ++j ) {
    //         printf( "%f ", *( TransA + i + ( j * nc ) ) );
    //     }
    //     printf( "\n" );
    // }

    return ierr;
}