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

    for ( base_int_t i = 0; i < FWHT_SHOWCASE_ROW_M * FWHT_SHOWCASE_COL_N; ++i ) {
        printf( "%f\n", data[i] );
    }
    printf( "\n" );

    return ierr;
}