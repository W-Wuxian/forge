#include "fwht.h"
#include "ubench.h"

// 1. Define your initialization helper function
void
set_mat_values( double *mat, int nele, int ncols )
{
    for ( int i = 0; i < nele; ++i ) {
        mat[i] = ( ( (double)i ) / ( (double)ncols ) ) + 3.14748;
    }
}

// 2. Define the macro, replacing the loop with your function call
#define BENCH_ROTATEDATA_MAT( NROWS, NCOLS, RIDX1, RIDX2 )                                         \
    UBENCH_EX( rotatedata_mat, dim_##NROWS##x##NCOLS##_rowidx1_##RIDX1##_rowidx2_##RIDX2 )         \
    {                                                                                              \
        base_int_t  nrows        = (base_int_t)NROWS;                                              \
        base_int_t  ncols        = (base_int_t)NCOLS;                                              \
        base_uint_t ridx1        = (base_int_t)RIDX1;                                              \
        base_uint_t ridx2        = (base_int_t)RIDX2;                                              \
        base_int_t  num_elements = nrows * ncols;                                                  \
        size_t      array_size   = (size_t)( num_elements * sizeof( double ) );                    \
        double     *data         = (double *)malloc( array_size );                                 \
                                                                                                   \
        /* Call your initialization function here */                                               \
        set_mat_values( data, num_elements, ncols );                                               \
                                                                                                   \
        /* ubench ONLY times the execution inside this block */                                    \
        UBENCH_DO_BENCHMARK()                                                                      \
        {                                                                                          \
            _base_rotatedata_mat( data, nrows, ncols, ridx1, ridx2 );                              \
        }                                                                                          \
                                                                                                   \
        free( data );                                                                              \
    }

#define BENCH_ROTATEDATA_MAT_V2( NROWS, NCOLS, RIDX1, RIDX2 )                                      \
    UBENCH_EX( rotatedata_mat_v2, dim_##NROWS##x##NCOLS##_rowidx1_##RIDX1##_rowidx2_##RIDX2 )      \
    {                                                                                              \
        base_int_t  nrows        = (base_int_t)NROWS;                                              \
        base_int_t  ncols        = (base_int_t)NCOLS;                                              \
        base_uint_t ridx1        = (base_int_t)RIDX1;                                              \
        base_uint_t ridx2        = (base_int_t)RIDX2;                                              \
        base_int_t  num_elements = nrows * ncols;                                                  \
        size_t      array_size   = (size_t)( num_elements * sizeof( double ) );                    \
        double     *data         = (double *)malloc( array_size );                                 \
                                                                                                   \
        /* Call your initialization function here */                                               \
        set_mat_values( data, num_elements, ncols );                                               \
                                                                                                   \
        /* ubench ONLY times the execution inside this block */                                    \
        UBENCH_DO_BENCHMARK()                                                                      \
        {                                                                                          \
            _base_rotatedata_mat_v2( data, NULL, nrows, ncols, ridx1, ridx2 );                     \
        }                                                                                          \
                                                                                                   \
        free( data );                                                                              \
    }

#define BENCH_ROTATEDATA_MAT_RMAJ( NROWS, NCOLS, RIDX1, RIDX2 )                                    \
    UBENCH_EX( rotatedata_mat_rmaj, dim_##NROWS##x##NCOLS##_rowidx1_##RIDX1##_rowidx2_##RIDX2 )    \
    {                                                                                              \
        base_int_t  nrows        = (base_int_t)NROWS;                                              \
        base_int_t  ncols        = (base_int_t)NCOLS;                                              \
        base_uint_t ridx1        = (base_int_t)RIDX1;                                              \
        base_uint_t ridx2        = (base_int_t)RIDX2;                                              \
        base_int_t  num_elements = nrows * ncols;                                                  \
        size_t      array_size   = (size_t)( num_elements * sizeof( double ) );                    \
        double     *data         = (double *)malloc( array_size );                                 \
                                                                                                   \
        set_mat_values( data, num_elements, ncols );                                               \
                                                                                                   \
        UBENCH_DO_BENCHMARK()                                                                      \
        {                                                                                          \
            _base_rotatedata_mat_rmaj( data, nrows, ncols, ridx1, ridx2 );                         \
        }                                                                                          \
                                                                                                   \
        free( data );                                                                              \
    }

// 3. Generate benchmarks for any dimensions (square or rectangular)
// 2^10
BENCH_ROTATEDATA_MAT( JUBE_NROWS, JUBE_NCOLS, JUBE_RIDX1, JUBE_RIDX2 )
BENCH_ROTATEDATA_MAT_V2( JUBE_NROWS, JUBE_NCOLS, JUBE_RIDX1, JUBE_RIDX2 )
BENCH_ROTATEDATA_MAT_RMAJ( JUBE_NROWS, JUBE_NCOLS, JUBE_RIDX1, JUBE_RIDX2 )

// 4. Generate the main() function
UBENCH_MAIN()