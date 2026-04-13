#include "fwht_tester.h"

int
main()
{
    base_int_t ierr = 0;

    ierr = test_base_dummy_fwht();

    if ( ierr != 0 ) {
        printf( "test_base_dummy_fwht failed.\n" );
    }

    ierr = 0;
    ierr = test_base_fwht_mat();

    if ( ierr != 0 ) {
        printf( "test_base_fwht_mat failed.\n" );
    }

    return ierr;
}