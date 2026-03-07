#include "fwht.h"

#ifndef M
#define M 32
#endif

#ifndef N
#define N 1
#endif

// ref solution https://en.wikipedia.org/wiki/Fast_Walsh%E2%80%93Hadamard_transform

int
main()
{
    base_int_t ierr = 0;

    double data[M * N];
    data[0] = 1.;
    data[1] = 0.;
    data[2] = 1.;
    data[3] = 0.;
    data[4] = 0.;
    data[5] = 1.;
    data[6] = 1.;
    data[7] = 0.;
    base_dummy_fwht( &data[0], M * N );

    for ( base_int_t i = 0; i < M * N; ++i ) {
        printf( "%f\n", data[i] );
    }
    printf( "\n" );

    return ierr;
}