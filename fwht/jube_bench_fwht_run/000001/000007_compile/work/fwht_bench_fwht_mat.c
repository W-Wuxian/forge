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

#define BENCH_FWHT_MAT( NROWS, NCOLS )                                                                                                                                                                 \
    UBENCH_EX( fwht_mat, dim_##NROWS##x##NCOLS )                                                                                                                                                       \
    {                                                                                                                                                                                                  \
        base_int_t nrows        = (base_int_t)NROWS;                                                                                                                                                   \
        base_int_t ncols        = (base_int_t)NCOLS;                                                                                                                                                   \
        base_int_t num_elements = nrows * ncols;                                                                                                                                                       \
        size_t     array_size   = (size_t)( num_elements * sizeof( double ) );                                                                                                                         \
        double    *data         = (double *)malloc( array_size );                                                                                                                                      \
                                                                                                                                                                                                       \
        /* Call your initialization function here */                                                                                                                                                   \
        set_mat_values( data, num_elements, ncols );                                                                                                                                                   \
                                                                                                                                                                                                       \
        /* ubench ONLY times the execution inside this block */                                                                                                                                        \
        UBENCH_DO_BENCHMARK()                                                                                                                                                                          \
        {                                                                                                                                                                                              \
            base_fwht_mat( data, nrows, ncols );                                                                                                                                                       \
        }                                                                                                                                                                                              \
                                                                                                                                                                                                       \
        free( data );                                                                                                                                                                                  \
        data = NULL;                                                                                                                                                                                   \
    }

#define BENCH_FWHT_MAT_V3( NROWS, NCOLS )                                                                                                                                                              \
    UBENCH_EX( fwht_mat_v3, dim_##NROWS##x##NCOLS )                                                                                                                                                    \
    {                                                                                                                                                                                                  \
        base_int_t nrows        = (base_int_t)NROWS;                                                                                                                                                   \
        base_int_t ncols        = (base_int_t)NCOLS;                                                                                                                                                   \
        base_int_t num_elements = nrows * ncols;                                                                                                                                                       \
        size_t     array_size   = (size_t)( num_elements * sizeof( double ) );                                                                                                                         \
        double    *data         = (double *)malloc( array_size );                                                                                                                                      \
                                                                                                                                                                                                       \
        /* Call your initialization function here */                                                                                                                                                   \
        set_mat_values( data, num_elements, ncols );                                                                                                                                                   \
                                                                                                                                                                                                       \
        /* ubench ONLY times the execution inside this block */                                                                                                                                        \
        UBENCH_DO_BENCHMARK()                                                                                                                                                                          \
        {                                                                                                                                                                                              \
            base_fwht_mat_v3( data, NULL, nrows, ncols );                                                                                                                                              \
        }                                                                                                                                                                                              \
                                                                                                                                                                                                       \
        free( data );                                                                                                                                                                                  \
        data = NULL;                                                                                                                                                                                   \
    }

#define BENCH_FWHT_MAT_V3_PREALLOC( NROWS, NCOLS )                                                                                                                                                     \
    UBENCH_EX( fwht_mat_v3_prealloc, dim_##NROWS##x##NCOLS )                                                                                                                                           \
    {                                                                                                                                                                                                  \
        base_int_t nrows        = (base_int_t)NROWS;                                                                                                                                                   \
        base_int_t ncols        = (base_int_t)NCOLS;                                                                                                                                                   \
        base_int_t num_elements = nrows * ncols;                                                                                                                                                       \
        size_t     array_size   = (size_t)( num_elements * sizeof( double ) );                                                                                                                         \
        size_t     buffer_size  = (size_t)( ncols * 2 * sizeof( double ) );                                                                                                                            \
        double    *data         = (double *)malloc( array_size );                                                                                                                                      \
        double    *buffer       = (double *)malloc( buffer_size );                                                                                                                                     \
                                                                                                                                                                                                       \
        /* Call your initialization function here */                                                                                                                                                   \
        set_mat_values( data, num_elements, ncols );                                                                                                                                                   \
                                                                                                                                                                                                       \
        /* ubench ONLY times the execution inside this block */                                                                                                                                        \
        UBENCH_DO_BENCHMARK()                                                                                                                                                                          \
        {                                                                                                                                                                                              \
            base_fwht_mat_v3( data, buffer, nrows, ncols );                                                                                                                                            \
        }                                                                                                                                                                                              \
                                                                                                                                                                                                       \
        free( data );                                                                                                                                                                                  \
        free( buffer );                                                                                                                                                                                \
        data   = NULL;                                                                                                                                                                                 \
        buffer = NULL;                                                                                                                                                                                 \
    }

#define BENCH_FWHT_MAT_RMAJ( NROWS, NCOLS )                                                                                                                                                            \
    UBENCH_EX( fwht_mat_rmaj, dim_##NROWS##x##NCOLS )                                                                                                                                                  \
    {                                                                                                                                                                                                  \
        base_int_t nrows        = (base_int_t)NROWS;                                                                                                                                                   \
        base_int_t ncols        = (base_int_t)NCOLS;                                                                                                                                                   \
        base_int_t num_elements = nrows * ncols;                                                                                                                                                       \
        size_t     array_size   = (size_t)( num_elements * sizeof( double ) );                                                                                                                         \
        double    *data         = (double *)malloc( array_size );                                                                                                                                      \
                                                                                                                                                                                                       \
        set_mat_values( data, num_elements, ncols );                                                                                                                                                   \
                                                                                                                                                                                                       \
        UBENCH_DO_BENCHMARK()                                                                                                                                                                          \
        {                                                                                                                                                                                              \
            base_fwht_mat_rmaj( data, nrows, ncols );                                                                                                                                                  \
        }                                                                                                                                                                                              \
                                                                                                                                                                                                       \
        free( data );                                                                                                                                                                                  \
        data = NULL;                                                                                                                                                                                   \
    }

#define BENCH_FWHT_MAT_RMAJ_V2( NROWS, NCOLS )                                                                                                                                                         \
    UBENCH_EX( fwht_mat_rmaj_v2, dim_##NROWS##x##NCOLS )                                                                                                                                               \
    {                                                                                                                                                                                                  \
        base_int_t nrows        = (base_int_t)NROWS;                                                                                                                                                   \
        base_int_t ncols        = (base_int_t)NCOLS;                                                                                                                                                   \
        base_int_t num_elements = nrows * ncols;                                                                                                                                                       \
        size_t     array_size   = (size_t)( num_elements * sizeof( double ) );                                                                                                                         \
        double    *data         = (double *)malloc( array_size );                                                                                                                                      \
                                                                                                                                                                                                       \
        set_mat_values( data, num_elements, ncols );                                                                                                                                                   \
                                                                                                                                                                                                       \
        UBENCH_DO_BENCHMARK()                                                                                                                                                                          \
        {                                                                                                                                                                                              \
            base_fwht_mat_rmaj_v2( data, nrows, ncols );                                                                                                                                               \
        }                                                                                                                                                                                              \
                                                                                                                                                                                                       \
        free( data );                                                                                                                                                                                  \
        data = NULL;                                                                                                                                                                                   \
    }

#define BENCH_FWHT_MAT_RMAJ_V3( NROWS, NCOLS )                                                                                                                                                         \
    UBENCH_EX( fwht_mat_rmaj_v3, dim_##NROWS##x##NCOLS )                                                                                                                                               \
    {                                                                                                                                                                                                  \
        base_int_t nrows        = (base_int_t)NROWS;                                                                                                                                                   \
        base_int_t ncols        = (base_int_t)NCOLS;                                                                                                                                                   \
        base_int_t num_elements = nrows * ncols;                                                                                                                                                       \
        size_t     array_size   = (size_t)( num_elements * sizeof( double ) );                                                                                                                         \
        double    *data         = (double *)malloc( array_size );                                                                                                                                      \
                                                                                                                                                                                                       \
        set_mat_values( data, num_elements, ncols );                                                                                                                                                   \
                                                                                                                                                                                                       \
        UBENCH_DO_BENCHMARK()                                                                                                                                                                          \
        {                                                                                                                                                                                              \
            base_fwht_mat_rmaj_v3( data, nrows, ncols );                                                                                                                                               \
        }                                                                                                                                                                                              \
                                                                                                                                                                                                       \
        free( data );                                                                                                                                                                                  \
        data = NULL;                                                                                                                                                                                   \
    }

#define BENCH_FWHT_MAT_RMAJ_V4( NROWS, NCOLS )                                                                                                                                                         \
    UBENCH_EX( fwht_mat_rmaj_v4, dim_##NROWS##x##NCOLS )                                                                                                                                               \
    {                                                                                                                                                                                                  \
        base_int_t nrows        = (base_int_t)NROWS;                                                                                                                                                   \
        base_int_t ncols        = (base_int_t)NCOLS;                                                                                                                                                   \
        base_int_t num_elements = nrows * ncols;                                                                                                                                                       \
        size_t     array_size   = (size_t)( num_elements * sizeof( double ) );                                                                                                                         \
        double    *data         = (double *)malloc( array_size );                                                                                                                                      \
                                                                                                                                                                                                       \
        set_mat_values( data, num_elements, ncols );                                                                                                                                                   \
                                                                                                                                                                                                       \
        UBENCH_DO_BENCHMARK()                                                                                                                                                                          \
        {                                                                                                                                                                                              \
            base_fwht_mat_rmaj_v4( data, nrows, ncols );                                                                                                                                               \
        }                                                                                                                                                                                              \
                                                                                                                                                                                                       \
        free( data );                                                                                                                                                                                  \
        data = NULL;                                                                                                                                                                                   \
    }

#define BENCH_FWHT_MAT_RMAJ_V31( NROWS, NCOLS )                                                                                                                                                        \
    UBENCH_EX( fwht_mat_rmaj_v31, dim_##NROWS##x##NCOLS )                                                                                                                                              \
    {                                                                                                                                                                                                  \
        base_int_t nrows        = (base_int_t)NROWS;                                                                                                                                                   \
        base_int_t ncols        = (base_int_t)NCOLS;                                                                                                                                                   \
        base_int_t num_elements = nrows * ncols;                                                                                                                                                       \
        size_t     array_size   = (size_t)( num_elements * sizeof( double ) );                                                                                                                         \
        double    *data         = NULL;                                                                                                                                                                \
        posix_memalign( (void **)&data, 64, array_size );                                                                                                                                              \
                                                                                                                                                                                                       \
        set_mat_values( data, num_elements, ncols );                                                                                                                                                   \
                                                                                                                                                                                                       \
        UBENCH_DO_BENCHMARK()                                                                                                                                                                          \
        {                                                                                                                                                                                              \
            base_fwht_mat_rmaj_v3( data, nrows, ncols );                                                                                                                                               \
        }                                                                                                                                                                                              \
                                                                                                                                                                                                       \
        free( data );                                                                                                                                                                                  \
        data = NULL;                                                                                                                                                                                   \
    }

#define BENCH_FWHT_MAT_RMAJ_V41( NROWS, NCOLS )                                                                                                                                                        \
    UBENCH_EX( fwht_mat_rmaj_v41, dim_##NROWS##x##NCOLS )                                                                                                                                              \
    {                                                                                                                                                                                                  \
        base_int_t nrows        = (base_int_t)NROWS;                                                                                                                                                   \
        base_int_t ncols        = (base_int_t)NCOLS;                                                                                                                                                   \
        base_int_t num_elements = nrows * ncols;                                                                                                                                                       \
        size_t     array_size   = (size_t)( num_elements * sizeof( double ) );                                                                                                                         \
        double    *data         = NULL;                                                                                                                                                                \
        posix_memalign( (void **)&data, 64, array_size );                                                                                                                                              \
                                                                                                                                                                                                       \
        set_mat_values( data, num_elements, ncols );                                                                                                                                                   \
                                                                                                                                                                                                       \
        UBENCH_DO_BENCHMARK()                                                                                                                                                                          \
        {                                                                                                                                                                                              \
            base_fwht_mat_rmaj_v4( data, nrows, ncols );                                                                                                                                               \
        }                                                                                                                                                                                              \
                                                                                                                                                                                                       \
        free( data );                                                                                                                                                                                  \
        data = NULL;                                                                                                                                                                                   \
    }

#define BENCH_FWHT_MAT_RMAJ_V5( NROWS, NCOLS )                                                                                                                                                         \
    UBENCH_EX( fwht_mat_rmaj_v5, dim_##NROWS##x##NCOLS )                                                                                                                                               \
    {                                                                                                                                                                                                  \
        base_int_t nrows        = (base_int_t)NROWS;                                                                                                                                                   \
        base_int_t ncols        = (base_int_t)NCOLS;                                                                                                                                                   \
        base_int_t num_elements = nrows * ncols;                                                                                                                                                       \
        size_t     array_size   = (size_t)( num_elements * sizeof( double ) );                                                                                                                         \
        double    *data         = NULL;                                                                                                                                                                \
        posix_memalign( (void **)&data, 64, array_size );                                                                                                                                              \
                                                                                                                                                                                                       \
        set_mat_values( data, num_elements, ncols );                                                                                                                                                   \
                                                                                                                                                                                                       \
        UBENCH_DO_BENCHMARK()                                                                                                                                                                          \
        {                                                                                                                                                                                              \
            base_fwht_mat_rmaj_v5( data, nrows, ncols );                                                                                                                                               \
        }                                                                                                                                                                                              \
                                                                                                                                                                                                       \
        free( data );                                                                                                                                                                                  \
        data = NULL;                                                                                                                                                                                   \
    }

#define BENCH_FWHT_FFTW_MEASURE_MAT( NROWS, NCOLS )                                                                                                                                                    \
    UBENCH_EX( fwht_fftw_measure_mat, dim_##NROWS##x##NCOLS )                                                                                                                                          \
    {                                                                                                                                                                                                  \
        base_int_t nrows        = (base_int_t)NROWS;                                                                                                                                                   \
        base_int_t ncols        = (base_int_t)NCOLS;                                                                                                                                                   \
        base_int_t num_elements = nrows * ncols;                                                                                                                                                       \
        view_t     vIn          = { .m = nrows, .n = ncols, .st1 = 1, .st2 = nrows };                                                                                                                  \
        size_t     array_size   = (size_t)( num_elements * sizeof( double ) );                                                                                                                         \
        double    *data         = (double *)malloc( array_size );                                                                                                                                      \
        fftw_plan  Hadaplan;                                                                                                                                                                           \
        base_SetFFTW( &Hadaplan, &vIn, 1, &data[0], &data[0] );                                                                                                                                        \
        /* Call your initialization function here */                                                                                                                                                   \
        set_mat_values( data, num_elements, ncols );                                                                                                                                                   \
                                                                                                                                                                                                       \
        /* ubench ONLY times the execution inside this block */                                                                                                                                        \
        UBENCH_DO_BENCHMARK()                                                                                                                                                                          \
        {                                                                                                                                                                                              \
            fftw_execute_r2r( Hadaplan, &data[0], &data[0] );                                                                                                                                          \
        }                                                                                                                                                                                              \
        base_FreeFFTW( &Hadaplan );                                                                                                                                                                    \
        free( data );                                                                                                                                                                                  \
        data = NULL;                                                                                                                                                                                   \
    }

#define BENCH_FWHT_FFTW_ESTIMATE_MAT( NROWS, NCOLS )                                                                                                                                                   \
    UBENCH_EX( fwht_fftw_estimate_mat, dim_##NROWS##x##NCOLS )                                                                                                                                         \
    {                                                                                                                                                                                                  \
        base_int_t nrows        = (base_int_t)NROWS;                                                                                                                                                   \
        base_int_t ncols        = (base_int_t)NCOLS;                                                                                                                                                   \
        base_int_t num_elements = nrows * ncols;                                                                                                                                                       \
        view_t     vIn          = { .m = nrows, .n = ncols, .st1 = 1, .st2 = nrows };                                                                                                                  \
        size_t     array_size   = (size_t)( num_elements * sizeof( double ) );                                                                                                                         \
        double    *data         = (double *)malloc( array_size );                                                                                                                                      \
        fftw_plan  Hadaplan;                                                                                                                                                                           \
        base_SetFFTW( &Hadaplan, &vIn, 0, &data[0], &data[0] );                                                                                                                                        \
        /* Call your initialization function here */                                                                                                                                                   \
        set_mat_values( data, num_elements, ncols );                                                                                                                                                   \
                                                                                                                                                                                                       \
        /* ubench ONLY times the execution inside this block */                                                                                                                                        \
        UBENCH_DO_BENCHMARK()                                                                                                                                                                          \
        {                                                                                                                                                                                              \
            fftw_execute_r2r( Hadaplan, &data[0], &data[0] );                                                                                                                                          \
        }                                                                                                                                                                                              \
        base_FreeFFTW( &Hadaplan );                                                                                                                                                                    \
        free( data );                                                                                                                                                                                  \
        data = NULL;                                                                                                                                                                                   \
    }

// 3. Generate benchmarks for any dimensions (square or rectangular)
BENCH_FWHT_MAT( 2048, 8192 )
BENCH_FWHT_MAT_V3( 2048, 8192 )
BENCH_FWHT_MAT_V3_PREALLOC( 2048, 8192 )
BENCH_FWHT_MAT_RMAJ( 2048, 8192 )
BENCH_FWHT_MAT_RMAJ_V2( 2048, 8192 )
BENCH_FWHT_MAT_RMAJ_V3( 2048, 8192 )
BENCH_FWHT_MAT_RMAJ_V31( 2048, 8192 )
BENCH_FWHT_MAT_RMAJ_V4( 2048, 8192 )
BENCH_FWHT_MAT_RMAJ_V41( 2048, 8192 )
BENCH_FWHT_MAT_RMAJ_V5( 2048, 8192 )
BENCH_FWHT_FFTW_MEASURE_MAT( 2048, 8192 )
BENCH_FWHT_FFTW_ESTIMATE_MAT( 2048, 8192 )

// 4. Generate the main() function
UBENCH_MAIN()