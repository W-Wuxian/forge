#include "fwht.h"

void
_base_rotatedata( double *data, base_uint_t c1, base_uint_t c2 )
{
    static double base_rotation;
    base_rotation = data[c1];
    data[c1]      = base_rotation + data[c2];
    data[c2]      = base_rotation - data[c2];
}

void
base_dummy_fwht( double *data, base_uint_t n )
{
    // Require n to be a power of 2
    base_uint_t l2 = base_ilog2( n ) - 1;
    base_uint_t l3 = 1 << l2;
    BASE_ASSERT_INT( n, l3 );
    // Compute the FWHT
    base_uint_t tmp;
    base_uint_t i;
    base_uint_t j;
    base_uint_t k;
    for ( i = 0; i < l2; ++i ) {
        l3 = (base_uint_t)( 1 << i );
        for ( j = 0; j < n; j += ( 1 << ( i + 1 ) ) ) {
            for ( k = 0; k < l3; ++k ) {
                tmp = j + k;
                _base_rotatedata( data, tmp, tmp + l3 );
            }
        }
    }
}