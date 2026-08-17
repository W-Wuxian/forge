#include "sketch.h"
#include "ubench.h"

void
set_mat_values( double *mat, base_int_t nele, base_int_t ncols )
{
    base_int_t i = 0;
    for ( ; i < nele; ++i ) {
        mat[i] = ( ( (double)i ) / ( (double)10 * (double)ncols ) ) + (double)( nele - i );
    }
}

#define BENCH_SKECTH_BLOCK_NOCOMM( NROWS, NCOLS, SKETCH_DIM, SKETCH_ALG, SKETCH_TYPE, NT )                                                                                                             \
    UBENCH_EX( sketch_block_nocomm, dim_##NROWS##x##NCOLS##_sketch_dim_##SKETCH_DIM##_sketch_alg_##SKETCH_ALG##_sketch_type_##SKETCH_TYPE##_openmp_##NT )                                              \
    {                                                                                                                                                                                                  \
        base_int_t    sketch_iparam[IDX_SKETCH_IPARAM_LEN];                                                                                                                                            \
        base_int_t    len_In   = 0;                                                                                                                                                                    \
        base_int_t    len_Out  = 0;                                                                                                                                                                    \
        size_t        size_In  = 0;                                                                                                                                                                    \
        size_t        size_Out = 0;                                                                                                                                                                    \
        base_sketch_t sketch_data;                                                                                                                                                                     \
        double       *In                      = NULL;                                                                                                                                                  \
        double       *Out                     = NULL;                                                                                                                                                  \
        sketch_iparam[IDX_NROWS_DATA_IN]      = NROWS;                                                                                                                                                 \
        sketch_iparam[IDX_NCOLS_DATA_IN]      = NCOLS;                                                                                                                                                 \
        sketch_iparam[IDX_SKETCH_DIM]         = SKETCH_DIM;                                                                                                                                            \
        sketch_iparam[IDX_NDL]                = 1;                                                                                                                                                     \
        sketch_iparam[IDX_NDR]                = 1;                                                                                                                                                     \
        sketch_iparam[IDX_SKETCH_ALG]         = SKETCH_ALG;                                                                                                                                            \
        sketch_iparam[IDX_SKETCH_TYPE]        = SKETCH_TYPE;                                                                                                                                           \
        sketch_iparam[IDX_SKETCH_NUM_THREADS] = NT;                                                                                                                                                    \
        sketch_iparam[IDX_SKETCH_NUM_GPU]     = 0;                                                                                                                                                     \
        len_In                                = sketch_iparam[IDX_NROWS_DATA_IN] * sketch_iparam[IDX_NCOLS_DATA_IN];                                                                                   \
        len_Out                               = sketch_iparam[IDX_SKETCH_DIM] * sketch_iparam[IDX_NCOLS_DATA_IN];                                                                                      \
        size_In                               = len_In * sizeof( double );                                                                                                                             \
        size_Out                              = len_Out * sizeof( double );                                                                                                                            \
        SPEALLOC( In, base_dalign, size_In );                                                                                                                                                          \
        assert( In != NULL );                                                                                                                                                                          \
        SPEALLOC( Out, base_dalign, size_Out );                                                                                                                                                        \
        assert( Out != NULL );                                                                                                                                                                         \
        base_init_sketch_data( &sketch_data, &sketch_iparam[0], 0, 1 );                                                                                                                                \
        base_set_sketch_data( &sketch_data, 0, 1 );                                                                                                                                                    \
        set_mat_values( In, len_In, sketch_iparam[IDX_NCOLS_DATA_IN] );                                                                                                                                \
        memset( Out, 0, size_Out );                                                                                                                                                                    \
        base_getdata_sketch_data( &sketch_data, sketch_iparam[IDX_NCOLS_DATA_IN], In, Out );                                                                                            \
        UBENCH_DO_BENCHMARK()                                                                                                                                                                          \
        {                                                                                                                                                                                              \
            base_compute_block_sketch_nocomm( &sketch_data );                                                                                                                                          \
        }                                                                                                                                                                                              \
        base_free_sketch_data( &sketch_data );                                                                                                                                                         \
        free( In );                                                                                                                                                                                    \
        In = NULL;                                                                                                                                                                                     \
        free( Out );                                                                                                                                                                                   \
        Out = NULL;                                                                                                                                                                                    \
    }

BENCH_SKECTH_BLOCK_NOCOMM( JUBE_NROWS, JUBE_NCOLS, JUBE_SKETCH_DIM, JUBE_SKETCH_ALG, JUBE_SKETCH_TYPE, 1 )
BENCH_SKECTH_BLOCK_NOCOMM( JUBE_NROWS, JUBE_NCOLS, JUBE_SKETCH_DIM, JUBE_SKETCH_ALG, JUBE_SKETCH_TYPE, 4 )
// 4. Generate the main() function
UBENCH_MAIN()