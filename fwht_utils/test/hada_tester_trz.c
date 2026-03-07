// #include "hada.h"
#include "hada_tester.h"
int
main()
{
    base_int_t ierr = 0;

    base_int_t n = 0;
    base_int_t v = 0;

    n = 1;
    v = 0;
    if ( test_trz( n ) != v ) {
        PRINT_INFO();
        printf( "trz(%zu) = %zu != %zu\n", n, test_trz( n ), v );
    }

    n = 2;
    v = 1;
    if ( test_trz( n ) != v ) {
        PRINT_INFO();
        printf( "trz(%zu) = %zu != %zu\n", n, test_trz( n ), v );
    }

    n = 3;
    v = 0;
    if ( test_trz( n ) != v ) {
        PRINT_INFO();
        printf( "trz(%zu) = %zu != %zu\n", n, test_trz( n ), v );
    }

    n = 4;
    v = 2;
    if ( test_trz( n ) != v ) {
        PRINT_INFO();
        printf( "trz(%zu) = %zu != %zu\n", n, test_trz( n ), v );
    }

    n = 5;
    v = 0;
    if ( test_trz( n ) != v ) {
        PRINT_INFO();
        printf( "trz(%zu) = %zu != %zu\n", n, test_trz( n ), v );
    }

    n = 6;
    v = 1;
    if ( test_trz( n ) != v ) {
        PRINT_INFO();
        printf( "trz(%zu) = %zu != %zu\n", n, test_trz( n ), v );
    }

    n = 7;
    v = 0;
    if ( test_trz( n ) != v ) {
        PRINT_INFO();
        printf( "trz(%zu) = %zu != %zu\n", n, test_trz( n ), v );
    }

    n = 8;
    v = 3;
    if ( test_trz( n ) != v ) {
        PRINT_INFO();
        printf( "trz(%zu) = %zu != %zu\n", n, test_trz( n ), v );
    }

    n = 13;
    v = 0;
    if ( test_trz( n ) != v ) {
        PRINT_INFO();
        printf( "trz(%zu) = %zu != %zu\n", n, test_trz( n ), v );
    }

    n = 16;
    v = 4;
    if ( test_trz( n ) != v ) {
        PRINT_INFO();
        printf( "trz(%zu) = %zu != %zu\n", n, test_trz( n ), v );
    }

    n = 24;
    v = 3;
    if ( test_trz( n ) != v ) {
        PRINT_INFO();
        printf( "trz(%zu) = %zu != %zu\n", n, test_trz( n ), v );
    }

    n = 30;
    v = 1;
    if ( test_trz( n ) != v ) {
        PRINT_INFO();
        printf( "trz(%zu) = %zu != %zu\n", n, test_trz( n ), v );
    }

    n = 32;
    v = 5;
    if ( test_trz( n ) != v ) {
        PRINT_INFO();
        printf( "trz(%zu) = %zu != %zu\n", n, test_trz( n ), v );
    }

    n = 44;
    v = 2;
    if ( test_trz( n ) != v ) {
        PRINT_INFO();
        printf( "trz(%zu) = %zu != %zu\n", n, test_trz( n ), v );
    }

    n = 64;
    v = 6;
    if ( test_trz( n ) != v ) {
        PRINT_INFO();
        printf( "trz(%zu) = %zu != %zu\n", n, test_trz( n ), v );
    }

    n = 100;
    v = 2;
    if ( test_trz( n ) != v ) {
        PRINT_INFO();
        printf( "trz(%zu) = %zu != %zu\n", n, test_trz( n ), v );
    }

    n = 404;
    v = 2;
    if ( test_trz( n ) != v ) {
        PRINT_INFO();
        printf( "trz(%zu) = %zu != %zu\n", n, test_trz( n ), v );
    }

    return ierr;
}