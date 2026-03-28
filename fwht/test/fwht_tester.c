#include "fwht_tester.h"

// ref solution https://en.wikipedia.org/wiki/Fast_Walsh%E2%80%93Hadamard_transform
#define REF_SOL_IDX_0 ( (double)4 )
#define REF_SOL_IDX_1 ( (double)2 )
#define REF_SOL_IDX_2 ( (double)0 )
#define REF_SOL_IDX_3 ( (double)-2 )
#define REF_SOL_IDX_4 ( (double)0 )
#define REF_SOL_IDX_5 ( (double)2 )
#define REF_SOL_IDX_6 ( (double)0 )
#define REF_SOL_IDX_7 ( (double)2 )

static const double REF_SOL[] = { REF_SOL_IDX_0, REF_SOL_IDX_1, REF_SOL_IDX_2, REF_SOL_IDX_3,
                                  REF_SOL_IDX_4, REF_SOL_IDX_5, REF_SOL_IDX_6, REF_SOL_IDX_7 };

#define BLAS_ROTM_FLAG ( (double)-1.0 )
#define MODIFIED_GIVENS_TRANSFORMATION_MATRIX_IDX_00 ( (double)1.0 )
#define MODIFIED_GIVENS_TRANSFORMATION_MATRIX_IDX_10 ( (double)1.0 )
#define MODIFIED_GIVENS_TRANSFORMATION_MATRIX_IDX_01 ( (double)1.0 )
#define MODIFIED_GIVENS_TRANSFORMATION_MATRIX_IDX_11 ( (double)-1.0 )
// BLAS_ROTM_PARAM is {flag, h11, h21, h12, and h22 }
#define H11 MODIFIED_GIVENS_TRANSFORMATION_MATRIX_IDX_00
#define H21 MODIFIED_GIVENS_TRANSFORMATION_MATRIX_IDX_10
#define H12 MODIFIED_GIVENS_TRANSFORMATION_MATRIX_IDX_01
#define H22 MODIFIED_GIVENS_TRANSFORMATION_MATRIX_IDX_11
static const double BLAS_ROTM_PARAM[] = { BLAS_ROTM_FLAG, H11, H21, H12, H22 };

base_int_t
test_base_dummy_fwht()
{
    base_int_t ierr        = 0;
    base_int_t j           = 0;
    base_int_t ref_sol_dim = 8 * 1;  // M = 8 N = 1

    double data[ref_sol_dim];
    for ( base_int_t i = 0; i < ref_sol_dim; i++ ) {
        data[i] = (double)0;
    }
    data[0] = 1.;
    data[1] = 0.;
    data[2] = 1.;
    data[3] = 0.;
    data[4] = 0.;
    data[5] = 1.;
    data[6] = 1.;
    data[7] = 0.;
    base_dummy_fwht( &data[0], ref_sol_dim );

    for ( j = 0; j < ref_sol_dim; ++j ) {
        if ( data[j] != REF_SOL[j] ) {
            PRINT_INFO();
            printf( "data[%zu] = %f != %f\n", j, data[j], REF_SOL[j] );
            ierr += 1;
        }
    }

    return ierr;
}

// data is 2x2 matrice data[k] = d_ij = d_{j+i*2} i is row index and j is column index
// C/C++ is Row-Major data = {-2,3,-1,6} = {d_00, d_01, d_10, d_11}
//
// d_00  d_01     -2   3
//
// d_10  d_11     -1   6
//
//
base_int_t
test_rotatedata_mat()
{
    base_int_t ierr = 0;
    double     data[4];
    data[0] = (double)-2.;
    data[1] = (double)3.;
    data[2] = (double)-1.;
    data[3] = (double)6.;
    printf( "Input matrix - Row-Major format\n" );
    printf( "%f  %f\n", data[0], data[1] );
    printf( "%f  %f\n", data[2], data[3] );
    printf( "Output matrix - Row-Major format\n" );
    cblas_drotm( 2, &data[0], 1, &data[2], 1, &BLAS_ROTM_PARAM[0] );
    printf( "%f  %f\n", data[0], data[1] );
    printf( "%f  %f\n", data[2], data[3] );
    return ierr;
}