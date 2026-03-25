#include "fwht.h"

#ifndef M
#define M 16
#endif

#ifndef N
#define N 1
#endif

// ref solution https://en.wikipedia.org/wiki/Fast_Walsh%E2%80%93Hadamard_transform

int
main()
{
    base_int_t ierr        = 0;
    base_int_t ref_sol_dim = 8 * 1;  // M = 8 N = 1

    double data[M * N];
    for ( base_int_t i = 0; i < M * N; i++ ) {
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

    for ( base_int_t i = 0; i < M * N; ++i ) {
        printf( "%f\n", data[i] );
    }
    printf( "\n" );

    return ierr;
}