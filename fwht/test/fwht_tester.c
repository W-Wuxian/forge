#include "fwht_tester.h"

/**********************************************************************************************************************/
// ref solution https://en.wikipedia.org/wiki/Fast_Walsh%E2%80%93Hadamard_transform
// Testting wikipedia 1D fwht transform, setting reference solution:
#define REF_SOL_IDX_0 ( (double)4 )
#define REF_SOL_IDX_1 ( (double)2 )
#define REF_SOL_IDX_2 ( (double)0 )
#define REF_SOL_IDX_3 ( (double)-2 )
#define REF_SOL_IDX_4 ( (double)0 )
#define REF_SOL_IDX_5 ( (double)2 )
#define REF_SOL_IDX_6 ( (double)0 )
#define REF_SOL_IDX_7 ( (double)2 )

#define REF_INIT_IDX_0 ( (double)1 )
#define REF_INIT_IDX_1 ( (double)0 )
#define REF_INIT_IDX_2 ( (double)1 )
#define REF_INIT_IDX_3 ( (double)0 )
#define REF_INIT_IDX_4 ( (double)0 )
#define REF_INIT_IDX_5 ( (double)1 )
#define REF_INIT_IDX_6 ( (double)1 )
#define REF_INIT_IDX_7 ( (double)0 )

static const double REF_SOL[]  = { REF_SOL_IDX_0, REF_SOL_IDX_1, REF_SOL_IDX_2, REF_SOL_IDX_3, REF_SOL_IDX_4, REF_SOL_IDX_5, REF_SOL_IDX_6, REF_SOL_IDX_7 };
static const double REF_INIT[] = { REF_INIT_IDX_0, REF_INIT_IDX_1, REF_INIT_IDX_2, REF_INIT_IDX_3, REF_INIT_IDX_4, REF_INIT_IDX_5, REF_INIT_IDX_6, REF_INIT_IDX_7 };

/**********************************************************************************************************************/
/**********************************************************************************************************************/
// ref implementation
// https://www.intel.com/content/www/us/en/docs/onemkl/developer-reference-c/2025-2/cblas-rotm.html
// Testing cblas_?rotm with 2D array i.e matrix, setting cblas_drotm params:
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
// Setting Input matrix values:
#define ROTM_INPUT_MATRIX_D11 ( (double)-2 )
#define ROTM_INPUT_MATRIX_D12 ( (double)3 )
#define ROTM_INPUT_MATRIX_D21 ( (double)-1 )
#define ROTM_INPUT_MATRIX_D22 ( (double)6 )
// Setting Ref solution i.e Output matrix value:
#define ROTM_OUTPUT_MATRIX_D11 ( (double)-3 )
#define ROTM_OUTPUT_MATRIX_D12 ( (double)9 )
#define ROTM_OUTPUT_MATRIX_D21 ( (double)-1 )
#define ROTM_OUTPUT_MATRIX_D22 ( (double)-3 )
/**********************************************************************************************************************/

base_int_t
test_base_dummy_fwht()
{
    base_int_t ierr        = 0;
    base_int_t j           = 0;
    base_int_t ref_sol_dim = 8 * 1;  // M = 8 N = 1

    double data[ref_sol_dim];
    // for ( base_int_t i = 0; i < ref_sol_dim; i++ ) {
    //     data[i] = REF_INIT[i];
    // }
    FILL_COLMAJ_MAT_WITH_VEC( &data[0], &REF_INIT[0], 8, 1 );
    PRINT_COLMAJ_MAT( &data[0], 8, 1, "base_dummy_fwht :: Initialized data is" );
    base_dummy_fwht( &data[0], ref_sol_dim );
    PRINT_COLMAJ_MAT( &data[0], 8, 1, "base_dummy_fwht :: Solution is" );
    // for ( j = 0; j < ref_sol_dim; ++j ) {
    //     if ( data[j] != REF_SOL[j] ) {
    //         PRINT_INFO();
    //         printf( "data[%zu] = %f != %f\n", j, data[j], REF_SOL[j] );
    //         ierr += 1;
    //     }
    // }
    CHECK_COLMAJ_MAT_WITH_VEC( data, REF_SOL, 8, 1, ierr );

    return ierr;
}

base_int_t
test_base_fwht_mat()
{
    base_int_t ierr = 0;
    base_int_t m = 8, n = 3;
    base_int_t ref_sol_dim = m * n;

    double data[ref_sol_dim];
    for ( base_int_t i = 0; i < ref_sol_dim; i++ ) {
        data[i] = (double)0;
    }
    FILL_COLMAJ_MAT_WITH_VEC( &data[0], &REF_INIT[0], m, n );
    PRINT_COLMAJ_MAT( &data[0], m, n, "base_fwht_mat :: Initialized data:" );
    // for ( base_int_t i = 0; i < n; i++ ) {
    //     data[0 + i * m] = 1.;
    //     data[1 + i * m] = 0.;
    //     data[2 + i * m] = 1.;
    //     data[3 + i * m] = 0.;
    //     data[4 + i * m] = 0.;
    //     data[5 + i * m] = 1.;
    //     data[6 + i * m] = 1.;
    //     data[7 + i * m] = 0.;
    // }
    base_fwht_mat( &data[0], m, n );
    PRINT_COLMAJ_MAT( &data[0], m, n, "base_fwht_mat :: Solution is:" );

    // printf( "data matrix Col-Major - Using base_fwht_mat method\n" );
    // for ( base_int_t i = 0; i < m; ++i ) {
    //     for ( base_int_t j = 0; j < n; ++j ) {
    //         printf( "%f ", *( data + i + ( j * m ) ) );
    //     }
    //     printf( "\n" );
    // }
    // for ( base_int_t i = 0; i < n; i++ ) {
    //     for ( base_int_t j = 0; j < m; j++ ) {
    //         if ( data[j + i * m] != REF_SOL[j] ) {
    //             PRINT_INFO();
    //             printf( "data[%zu] = %f != %f\n", j, data[j + i * m], REF_SOL[j] );
    //             ierr += 1;
    //         }
    //     }
    // }
    printf( "------------------------------------------------------------\n" );
    double *buffer = (double *)malloc( sizeof( double ) * n * 2 );
    FILL_COLMAJ_MAT_WITH_VEC( &data[0], &REF_INIT[0], m, n );
    PRINT_COLMAJ_MAT( &data[0], m, n, "base_fwht_mat_v3 :: Initialized data:" );
    // for ( base_int_t i = 0; i < n; i++ ) {
    //     data[0 + i * m] = 1.;
    //     data[1 + i * m] = 0.;
    //     data[2 + i * m] = 1.;
    //     data[3 + i * m] = 0.;
    //     data[4 + i * m] = 0.;
    //     data[5 + i * m] = 1.;
    //     data[6 + i * m] = 1.;
    //     data[7 + i * m] = 0.;
    // }
    base_fwht_mat_v3( &data[0], buffer, m, n );
    free( buffer );
    buffer = NULL;
    PRINT_COLMAJ_MAT( &data[0], m, n, "base_fwht_mat_v3 :: Solution is:" );
    // printf( "data matrix Col-Major - Using base_fwht_mat_v3 method\n" );
    // for ( base_int_t i = 0; i < m; ++i ) {
    //     for ( base_int_t j = 0; j < n; ++j ) {
    //         printf( "%f ", *( data + i + ( j * m ) ) );
    //     }
    //     printf( "\n" );
    // }
    // for ( base_int_t i = 0; i < n; i++ ) {
    //     for ( base_int_t j = 0; j < m; j++ ) {
    //         if ( data[j + i * m] != REF_SOL[j] ) {
    //             PRINT_INFO();
    //             printf( "data[%zu] = %f != %f\n", j, data[j + i * m], REF_SOL[j] );
    //             ierr += 1;
    //         }
    //     }
    // }
    printf( "------------------------------------------------------------\n" );
    for ( base_int_t i = 0; i < n; ++i ) {
        data[i + 0 * n] = 1.;
        data[i + 1 * n] = 0.;
        data[i + 2 * n] = 1.;
        data[i + 3 * n] = 0.;
        data[i + 4 * n] = 0.;
        data[i + 5 * n] = 1.;
        data[i + 6 * n] = 1.;
        data[i + 7 * n] = 0.;
    }
    base_fwht_mat_rmaj( &data[0], m, n );
    printf( "data matrix Row-Major - Using base_fwht_mat_rmaj method\n" );
    for ( base_int_t i = 0; i < m; ++i ) {
        for ( base_int_t j = 0; j < n; ++j ) {
            printf( "%f ", *( data + j + ( i * n ) ) );
        }
        printf( "\n" );
    }
    for ( base_int_t i = 0; i < n; ++i ) {
        for ( base_int_t j = 0; j < m; ++j ) {
            if ( data[i + j * n] != REF_SOL[j] ) {
                PRINT_INFO();
                printf( "data[%zu] = %f != %f\n", j, data[i + j * n], REF_SOL[j] );
                ierr += 1;
            }
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
    double     data[4];           // Row-Major
    double     colmajor_data[4];  // Col-Major {-2,-1,3,6} = {d_00, d_10, d_01, d_11}
    /*********************************************************************************************************************************************/
    data[0] = ROTM_INPUT_MATRIX_D11;
    data[1] = ROTM_INPUT_MATRIX_D12;
    data[2] = ROTM_INPUT_MATRIX_D21;
    data[3] = ROTM_INPUT_MATRIX_D22;
    printf( "Input matrix - Row-Major format\n" );
    printf( "%f  %f\n", data[0], data[1] );
    printf( "%f  %f\n", data[2], data[3] );
    cblas_drotm( 2, &data[0], 1, &data[2], 1, &BLAS_ROTM_PARAM[0] );
    printf( "Output matrix - Row-Major format - Compute using cblas_drotm\n" );
    printf( "%f  %f\n", data[0], data[1] );
    printf( "%f  %f\n", data[2], data[3] );
    if ( data[0] != ROTM_OUTPUT_MATRIX_D11 && data[1] != ROTM_OUTPUT_MATRIX_D12 && data[2] != ROTM_OUTPUT_MATRIX_D21 && data[3] != ROTM_OUTPUT_MATRIX_D22 ) {
        ierr += 1;
    }
    /*********************************************************************************************************************************************/
    /*********************************************************************************************************************************************/
    data[0] = ROTM_INPUT_MATRIX_D11;
    data[1] = ROTM_INPUT_MATRIX_D12;
    data[2] = ROTM_INPUT_MATRIX_D21;
    data[3] = ROTM_INPUT_MATRIX_D22;
    printf( "Input matrix - Row-Major format\n" );
    printf( "%f  %f\n", data[0], data[1] );
    printf( "%f  %f\n", data[2], data[3] );
    for ( int i = 0; i < 2; ++i ) {
        _base_rotatedata( &data[0], i, i + 2 );
    }
    // _base_rotatedata( &data[0], 0, 2);
    // _base_rotatedata( &data[0], 1, 3);
    printf( "Output matrix - Row-Major format - Compute using rotatedata with loop\n" );
    printf( "%f  %f\n", data[0], data[1] );
    printf( "%f  %f\n", data[2], data[3] );
    if ( data[0] != ROTM_OUTPUT_MATRIX_D11 && data[1] != ROTM_OUTPUT_MATRIX_D12 && data[2] != ROTM_OUTPUT_MATRIX_D21 && data[3] != ROTM_OUTPUT_MATRIX_D22 ) {
        ierr += 1;
    }
    /*********************************************************************************************************************************************/
    /*********************************************************************************************************************************************/
    data[0] = ROTM_INPUT_MATRIX_D11;
    data[1] = ROTM_INPUT_MATRIX_D12;
    data[2] = ROTM_INPUT_MATRIX_D21;
    data[3] = ROTM_INPUT_MATRIX_D22;
    printf( "Input matrix - Row-Major format\n" );
    printf( "%f  %f\n", data[0], data[1] );
    printf( "%f  %f\n", data[2], data[3] );
    data[0] = H11 * ROTM_INPUT_MATRIX_D11 + H12 * ROTM_INPUT_MATRIX_D21;
    data[2] = H21 * ROTM_INPUT_MATRIX_D11 + H22 * ROTM_INPUT_MATRIX_D21;
    data[1] = H11 * ROTM_INPUT_MATRIX_D12 + H12 * ROTM_INPUT_MATRIX_D22;
    data[3] = H21 * ROTM_INPUT_MATRIX_D12 + H22 * ROTM_INPUT_MATRIX_D22;
    printf( "Output matrix - Row-Major format - Compute using Modified Givens rotation matrix\n" );
    printf( "%f  %f\n", data[0], data[1] );
    printf( "%f  %f\n", data[2], data[3] );
    if ( data[0] != ROTM_OUTPUT_MATRIX_D11 && data[1] != ROTM_OUTPUT_MATRIX_D12 && data[2] != ROTM_OUTPUT_MATRIX_D21 && data[3] != ROTM_OUTPUT_MATRIX_D22 ) {
        ierr += 1;
    }
    /*********************************************************************************************************************************************/
    colmajor_data[0] = ROTM_INPUT_MATRIX_D11;
    colmajor_data[1] = ROTM_INPUT_MATRIX_D21;
    colmajor_data[2] = ROTM_INPUT_MATRIX_D12;
    colmajor_data[3] = ROTM_INPUT_MATRIX_D22;
    printf( "Input matrix - Col-Major format\n" );
    printf( "%f  %f\n", colmajor_data[0], colmajor_data[2] );
    printf( "%f  %f\n", colmajor_data[1], colmajor_data[3] );
    _base_rotatedata_mat( &colmajor_data[0], 2, 2, 0, 1 );
    printf( "Output matrix - Col-Major format - Compute using _base_rotatedata_mat\n" );
    printf( "%f  %f\n", colmajor_data[0], colmajor_data[2] );
    printf( "%f  %f\n", colmajor_data[1], colmajor_data[3] );
    if ( colmajor_data[0] != ROTM_OUTPUT_MATRIX_D11 && colmajor_data[1] != ROTM_OUTPUT_MATRIX_D21 && colmajor_data[2] != ROTM_OUTPUT_MATRIX_D12 && colmajor_data[3] != ROTM_OUTPUT_MATRIX_D22 ) {
        ierr += 1;
    }
    /*********************************************************************************************************************************************/
    /*********************************************************************************************************************************************/
    colmajor_data[0] = ROTM_INPUT_MATRIX_D11;
    colmajor_data[1] = ROTM_INPUT_MATRIX_D21;
    colmajor_data[2] = ROTM_INPUT_MATRIX_D12;
    colmajor_data[3] = ROTM_INPUT_MATRIX_D22;
    printf( "Input matrix - Col-Major format\n" );
    printf( "%f  %f\n", colmajor_data[0], colmajor_data[2] );
    printf( "%f  %f\n", colmajor_data[1], colmajor_data[3] );
    _base_rotatedata_mat_v2( &colmajor_data[0], NULL, 2, 2, 0, 1 );
    printf( "Output matrix - Col-Major format - Compute using _base_rotatedata_mat_v2\n" );
    printf( "%f  %f\n", colmajor_data[0], colmajor_data[2] );
    printf( "%f  %f\n", colmajor_data[1], colmajor_data[3] );
    if ( colmajor_data[0] != ROTM_OUTPUT_MATRIX_D11 && colmajor_data[1] != ROTM_OUTPUT_MATRIX_D21 && colmajor_data[2] != ROTM_OUTPUT_MATRIX_D12 && colmajor_data[3] != ROTM_OUTPUT_MATRIX_D22 ) {
        ierr += 1;
    }
    /*********************************************************************************************************************************************/
    /*********************************************************************************************************************************************/
    colmajor_data[0] = ROTM_INPUT_MATRIX_D11;
    colmajor_data[1] = ROTM_INPUT_MATRIX_D21;
    colmajor_data[2] = ROTM_INPUT_MATRIX_D12;
    colmajor_data[3] = ROTM_INPUT_MATRIX_D22;
    printf( "Input matrix - Col-Major format\n" );
    printf( "%f  %f\n", colmajor_data[0], colmajor_data[2] );
    printf( "%f  %f\n", colmajor_data[1], colmajor_data[3] );
    double *buffer = (double *)malloc( sizeof( double ) * 2 * 2 );
    _base_rotatedata_mat_v2( &colmajor_data[0], buffer, 2, 2, 0, 1 );
    free( buffer );
    buffer = NULL;
    printf( "Output matrix - Col-Major format - Compute using _base_rotatedata_mat_v2_prealloc\n" );
    printf( "%f  %f\n", colmajor_data[0], colmajor_data[2] );
    printf( "%f  %f\n", colmajor_data[1], colmajor_data[3] );
    if ( colmajor_data[0] != ROTM_OUTPUT_MATRIX_D11 && colmajor_data[1] != ROTM_OUTPUT_MATRIX_D21 && colmajor_data[2] != ROTM_OUTPUT_MATRIX_D12 && colmajor_data[3] != ROTM_OUTPUT_MATRIX_D22 ) {
        ierr += 1;
    }
    /*********************************************************************************************************************************************/
    /*********************************************************************************************************************************************/
    colmajor_data[0] = ROTM_INPUT_MATRIX_D11;
    colmajor_data[1] = ROTM_INPUT_MATRIX_D21;
    colmajor_data[2] = ROTM_INPUT_MATRIX_D12;
    colmajor_data[3] = ROTM_INPUT_MATRIX_D22;
    printf( "Input matrix - Col-Major format\n" );
    printf( "%f  %f\n", colmajor_data[0], colmajor_data[2] );
    printf( "%f  %f\n", colmajor_data[1], colmajor_data[3] );
    _base_rotatedata_mat_v3( &colmajor_data[0], NULL, 2, 2, 0, 1 );
    printf( "Output matrix - Col-Major format - Compute using _base_rotatedata_mat_v3\n" );
    printf( "%f  %f\n", colmajor_data[0], colmajor_data[2] );
    printf( "%f  %f\n", colmajor_data[1], colmajor_data[3] );
    if ( colmajor_data[0] != ROTM_OUTPUT_MATRIX_D11 && colmajor_data[1] != ROTM_OUTPUT_MATRIX_D21 && colmajor_data[2] != ROTM_OUTPUT_MATRIX_D12 && colmajor_data[3] != ROTM_OUTPUT_MATRIX_D22 ) {
        ierr += 1;
    }
    /*********************************************************************************************************************************************/
    /*********************************************************************************************************************************************/
    colmajor_data[0] = ROTM_INPUT_MATRIX_D11;
    colmajor_data[1] = ROTM_INPUT_MATRIX_D21;
    colmajor_data[2] = ROTM_INPUT_MATRIX_D12;
    colmajor_data[3] = ROTM_INPUT_MATRIX_D22;
    printf( "Input matrix - Col-Major format\n" );
    printf( "%f  %f\n", colmajor_data[0], colmajor_data[2] );
    printf( "%f  %f\n", colmajor_data[1], colmajor_data[3] );
    buffer = (double *)malloc( sizeof( double ) * 2 * 2 );
    _base_rotatedata_mat_v3( &colmajor_data[0], buffer, 2, 2, 0, 1 );
    free( buffer );
    buffer = NULL;
    printf( "Output matrix - Col-Major format - Compute using _base_rotatedata_mat_v3_prealloc\n" );
    printf( "%f  %f\n", colmajor_data[0], colmajor_data[2] );
    printf( "%f  %f\n", colmajor_data[1], colmajor_data[3] );
    if ( colmajor_data[0] != ROTM_OUTPUT_MATRIX_D11 && colmajor_data[1] != ROTM_OUTPUT_MATRIX_D21 && colmajor_data[2] != ROTM_OUTPUT_MATRIX_D12 && colmajor_data[3] != ROTM_OUTPUT_MATRIX_D22 ) {
        ierr += 1;
    }
    /*********************************************************************************************************************************************/
    /*********************************************************************************************************************************************/
    data[0] = ROTM_INPUT_MATRIX_D11;
    data[1] = ROTM_INPUT_MATRIX_D12;
    data[2] = ROTM_INPUT_MATRIX_D21;
    data[3] = ROTM_INPUT_MATRIX_D22;
    printf( "Input matrix - Row-Major format\n" );
    printf( "%f  %f\n", data[0], data[1] );
    printf( "%f  %f\n", data[2], data[3] );
    _base_rotatedata_mat_rmaj( &data[0], 2, 2, 0, 1 );
    printf( "Output matrix - Row-Major format - Compute using _base_rotatedata_mat_rmaj\n" );
    printf( "%f  %f\n", data[0], data[1] );
    printf( "%f  %f\n", data[2], data[3] );
    if ( data[0] != ROTM_OUTPUT_MATRIX_D11 && data[1] != ROTM_OUTPUT_MATRIX_D12 && data[2] != ROTM_OUTPUT_MATRIX_D21 && data[3] != ROTM_OUTPUT_MATRIX_D22 ) {
        ierr += 1;
    }
    /*********************************************************************************************************************************************/
    /*********************************************************************************************************************************************/
    data[0] = ROTM_INPUT_MATRIX_D11;
    data[1] = ROTM_INPUT_MATRIX_D12;
    data[2] = ROTM_INPUT_MATRIX_D21;
    data[3] = ROTM_INPUT_MATRIX_D22;
    printf( "Input matrix - Row-Major format\n" );
    printf( "%f  %f\n", data[0], data[1] );
    printf( "%f  %f\n", data[2], data[3] );
    _base_rotatedata_mat_rmaj_v2( &data[0], 2, 2, 0, 1 );
    printf( "Output matrix - Row-Major format - Compute using _base_rotatedata_mat_rmaj_v2\n" );
    printf( "%f  %f\n", data[0], data[1] );
    printf( "%f  %f\n", data[2], data[3] );
    if ( data[0] != ROTM_OUTPUT_MATRIX_D11 && data[1] != ROTM_OUTPUT_MATRIX_D12 && data[2] != ROTM_OUTPUT_MATRIX_D21 && data[3] != ROTM_OUTPUT_MATRIX_D22 ) {
        ierr += 1;
    }
    /*********************************************************************************************************************************************/
    return ierr;
}