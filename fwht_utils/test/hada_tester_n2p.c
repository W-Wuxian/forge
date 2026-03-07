// #include "hada.h"
#include "hada_tester.h"
int
main()
{
    base_int_t ierr = 0;

    base_int_t  n = 0;
    base_uint_t v = 0;

    n = 2;
    v = 2;
    if ( test_n2p( n ) != v ) {
        PRINT_INFO();
        printf( "base_n2p(%zu) = %zu != %zu\n", n, base_n2p( n ), v );
    }

    n = 4;
    v = 4;
    if ( test_n2p( n ) != v ) {
        PRINT_INFO();
        printf( "base_n2p(%zu) = %zu != %zu\n", n, base_n2p( n ), v );
    }

    n = 5;
    v = 8;
    if ( test_n2p( n ) != v ) {
        PRINT_INFO();
        printf( "base_n2p(%zu) = %zu != %zu\n", n, base_n2p( n ), v );
    }

    n = 8;
    v = 8;
    if ( test_n2p( n ) != v ) {
        PRINT_INFO();
        printf( "base_n2p(%zu) = %zu != %zu\n", n, base_n2p( n ), v );
    }

    n = 16;
    v = 16;
    if ( test_n2p( n ) != v ) {
        PRINT_INFO();
        printf( "base_n2p(%zu) = %zu != %zu\n", n, base_n2p( n ), v );
    }

    return ierr;
}