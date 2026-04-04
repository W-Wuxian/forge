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

void
_base_rotatedata_mat_v2( double     *data,
                         double     *buffer,
                         base_int_t  nrows,
                         base_int_t  ncols,
                         base_uint_t ridx1,
                         base_uint_t ridx2 )
{
    // param := flag, h11, h21, h12, h22
    static const double rotatedata_mat_param[] = { -1.0, 1.0, 1.0, 1.0, -1.0 };
    // static double buffer[2*1024]; need loop to overcome/map dimension : to try
    double *_buffer = NULL;
    if ( buffer != NULL ) {
        _buffer = buffer;
    }
    else {
        _buffer = (double *)malloc( sizeof( double ) * 2 * ncols );
    }
    // Alias
    double *x = &data[ridx1];
    double *y = &data[ridx2];

    cblas_dcopy( ncols, x, nrows, _buffer, 1 );
#ifdef _BASE_ROTATEDATA_MAT_V2_DEBUG
    printf( "_buffer_x\n" );
    for ( int i = 0; i < nrows; ++i ) {
        printf( "%f ", _buffer[i] );
    }
    printf( "\n" );
#endif
    cblas_dcopy( ncols, y, nrows, _buffer + ncols, 1 );
#ifdef _BASE_ROTATEDATA_MAT_V2_DEBUG
    printf( "_buffer_y\n" );
    for ( int i = nrows; i < 2 * nrows; ++i ) {
        printf( "%f ", _buffer[i] );
    }
    printf( "\n" );
#endif

    x = _buffer;
#ifdef _BASE_ROTATEDATA_MAT_V2_DEBUG
    printf( "x\n" );
    for ( int i = 0; i < nrows; ++i ) {
        printf( "%f ", x[i] );
    }
    printf( "\n" );
#endif

    y = _buffer + ncols;
#ifdef _BASE_ROTATEDATA_MAT_V2_DEBUG
    printf( "y\n" );
    for ( int i = 0; i < nrows; ++i ) {
        printf( "%f ", y[i] );
    }
    printf( "\n" );
#endif

    // Performs modified Givens rotation of points in the plane
    cblas_drotm( ncols, x, 1, y, 1, rotatedata_mat_param );

    // copy back buffer to main memory
    x = &data[ridx1];
    y = &data[ridx2];
    cblas_dcopy( ncols, _buffer, 1, x, nrows );
    cblas_dcopy( ncols, _buffer + ncols, 1, y, nrows );

    x = NULL;
    y = NULL;
    if ( buffer == NULL ) {
        free( _buffer );
        _buffer = NULL;
    }
}

void
_base_rotatedata_mat_v3( double     *data,
                         double     *buffer,
                         base_int_t  nrows,
                         base_int_t  ncols,
                         base_uint_t ridx1,
                         base_uint_t ridx2 )
{
    // param := flag, h11, h21, h12, h22
    static const double rotatedata_mat_param[] = { -1.0, 1.0, 1.0, 1.0, -1.0 };
    
    double *_buffer = NULL;
    if ( buffer != NULL ) {
        _buffer = buffer;
    }
    else {
        _buffer = (double *)malloc( sizeof( double ) * ncols * 2 );
    }

    cblas_dcopy(ncols, data + ridx1, nrows, _buffer, 1);
    cblas_dcopy(ncols, data + ridx2, nrows, _buffer + ncols, 1);

    cblas_drotm(ncols, _buffer, 1, _buffer + ncols, 1, rotatedata_mat_param );

    cblas_dcopy(ncols, _buffer, 1, data + ridx1, nrows);
    cblas_dcopy(ncols, _buffer + ncols, 1, data + ridx2, nrows);

    if ( buffer == NULL ) {
        free( _buffer );
        _buffer = NULL;
    }
}

void
_base_rotatedata_mat_rmaj( double     *data,
                           base_int_t  nrows,
                           base_int_t  ncols,
                           base_uint_t ridx1,
                           base_uint_t ridx2 )
{
    // param := flag, h11, h21, h12, h22
    static const double rotatedata_mat_param[] = { -1.0, 1.0, 1.0, 1.0, -1.0 };
    // Alias
    double *x = &data[ridx1 * ncols];
    double *y = &data[ridx2 * ncols];
    // Performs modified Givens rotation of points in the plane
    cblas_drotm( ncols, x, 1, y, 1, rotatedata_mat_param );
}