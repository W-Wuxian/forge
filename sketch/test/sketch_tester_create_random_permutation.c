#include "sketch.h"

int
main()
{
    int        ierr         = 0;
    base_int_t input_len    = 12;
    base_int_t len_data_out = 7;
    base_int_t data_out[len_data_out];
    for ( base_int_t i = 0; i < len_data_out; ++i )
        data_out[i] = (double)0;
    PRINT_ARRAY( data_out, "%zu\n", len_data_out, "Init Zero" );
    _base_create_random_permutation( input_len, &data_out[0], len_data_out );
    PRINT_ARRAY( data_out, "%zu\n", len_data_out, "Permutation Array" );
    return ierr;
}