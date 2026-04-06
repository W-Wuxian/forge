// #include "hada.h"
#include "hada_tester.h"
int
main()
{
    base_int_t ierr = 0;

    base_int_t n = 0;
    bool       v = false;

    n = 2;
    v = true;
    if ( test_ispow2( n ) != v ) {
        PRINT_INFO();
        printf( "base_ispow2(%zu) = %s != %s\n", n, base_ispow2( n ) ? "true" : "false", v ? "true" : "false" );
    }

    n = 4;
    v = true;
    if ( test_ispow2( n ) != v ) {
        PRINT_INFO();
        printf( "base_ispow2(%zu) = %s != %s\n", n, base_ispow2( n ) ? "true" : "false", v ? "true" : "false" );
    }

    n = 8;
    v = true;
    if ( test_ispow2( n ) != v ) {
        PRINT_INFO();
        printf( "base_ispow2(%zu) = %s != %s\n", n, base_ispow2( n ) ? "true" : "false", v ? "true" : "false" );
    }

    n = 16;
    v = true;
    if ( test_ispow2( n ) != v ) {
        PRINT_INFO();
        printf( "base_ispow2(%zu) = %s != %s\n", n, base_ispow2( n ) ? "true" : "false", v ? "true" : "false" );
    }

    n = 19;
    v = false;
    if ( test_ispow2( n ) != v ) {
        PRINT_INFO();
        printf( "base_ispow2(%zu) = %s != %s\n", n, base_ispow2( n ) ? "true" : "false", v ? "true" : "false" );
    }

    return ierr;
}