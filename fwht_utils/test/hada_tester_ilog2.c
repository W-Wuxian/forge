// #include "hada.h"
#include "hada_tester.h"
int
main()
{
    base_int_t ierr = 0;

    base_int_t  n = 0;
    base_uint_t v = 0;

    n = 2;
    v = 1;
    if ( test_ilog2( n ) - 1 != v ) {
        PRINT_INFO();
        printf( "base_ilog2(%zu) = %zu != %zu\n", n, base_ilog2( n ) - 1, v );
    }

    n = 4;
    v = 2;
    if ( test_ilog2( n ) - 1 != v ) {
        PRINT_INFO();
        printf( "base_ilog2(%zu) = %zu != %zu\n", n, base_ilog2( n ) - 1, v );
    }

    n = 5;
    v = 2;
    if ( test_ilog2( n ) - 1 != v ) {
        PRINT_INFO();
        printf( "base_ilog2(%zu) = %zu != %zu\n", n, base_ilog2( n ) - 1, v );
    }

    n = 8;
    v = 3;
    if ( test_ilog2( n ) - 1 != v ) {
        PRINT_INFO();
        printf( "base_ilog2(%zu) = %zu != %zu\n", n, base_ilog2( n ) - 1, v );
    }

    n = 16;
    v = 4;
    if ( test_ilog2( n ) - 1 != v ) {
        PRINT_INFO();
        printf( "base_ilog2(%zu) = %zu != %zu\n", n, base_ilog2( n ) - 1, v );
    }

    return ierr;
}