#include "fwht_tester.h"

int
main()
{
    base_int_t ierr = 0;

    ierr = test_base_dummy_fwht();

    if ( ierr != 0 ) {
        printf( "test_base_dummy_fwht failed.\n" );
    }

    return ierr;
}