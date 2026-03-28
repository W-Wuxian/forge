#include "fwht.h"

void
_base_rotatedata( double *data, base_uint_t ridx1, base_uint_t ridx2 )
{
    static double base_rotation;
    base_rotation = data[ridx1];
    data[ridx1]   = base_rotation + data[ridx2];
    data[ridx2]   = base_rotation - data[ridx2];
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

void
_base_rotatedata_mat( double     *data,
                      base_int_t  nrows,
                      base_int_t  ncols,
                      base_uint_t ridx1,
                      base_uint_t ridx2 )
{
    // param := flag, h11, h21, h12, h22
    static const double rotatedata_mat_param[] = { -1.0, 1.0, 1.0, 1.0, -1.0 };
    // Alias
    double *x = &data[ridx1];
    double *y = &data[ridx2];
    // Performs modified Givens rotation of points in the plane
    cblas_drotm( ncols, x, nrows, y, nrows, rotatedata_mat_param );
}