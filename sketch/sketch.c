#include "sketch.h"

void
_base_create_random_permutation( base_int_t input_len, base_int_t *data_out, base_int_t len_data_out )
{
    base_int_t  i, j, sw;
    base_int_t *rand_tmp = NULL;
    rand_tmp             = (base_int_t *)malloc( sizeof( base_int_t ) * input_len );
    BASE_ASSERT_ISNOTNULL( rand_tmp );
    memset( rand_tmp, 0, sizeof( base_int_t ) * input_len );
    for ( i = 0; i < input_len; i++ )
        rand_tmp[i] = i;
    srand( 7654321 );
    for ( i = 0; i < len_data_out; i++ ) {
        j           = rand() % ( input_len - i ) + i;
        sw          = rand_tmp[i];
        rand_tmp[i] = rand_tmp[j];
        data_out[i] = rand_tmp[j];
        rand_tmp[j] = sw;
    }
    free( rand_tmp );
    rand_tmp = NULL;
}