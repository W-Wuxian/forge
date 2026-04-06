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
#define BENCH_ROTATEDATA_MAT( NROWS, NCOLS, RIDX1, RIDX2 )                                                                                                                                             \
    UBENCH_EX( rotatedata_mat, dim_##NROWS##x##NCOLS##_rowidx1_##RIDX1##_rowidx2_##RIDX2 )                                                                                                             \
    {                                                                                                                                                                                                  \
        base_int_t  nrows        = (base_int_t)NROWS;                                                                                                                                                  \
        base_int_t  ncols        = (base_int_t)NCOLS;                                                                                                                                                  \
        base_uint_t ridx1        = (base_int_t)RIDX1;                                                                                                                                                  \
        base_uint_t ridx2        = (base_int_t)RIDX2;                                                                                                                                                  \
        base_int_t  num_elements = nrows * ncols;                                                                                                                                                      \
        size_t      array_size   = (size_t)( num_elements * sizeof( double ) );                                                                                                                        \
        double     *data         = (double *)malloc( array_size );                                                                                                                                     \
                                                                                                                                                                                                       \
        /* Call your initialization function here */                                                                                                                                                   \
        set_mat_values( data, num_elements, ncols );                                                                                                                                                   \
                                                                                                                                                                                                       \
        /* ubench ONLY times the execution inside this block */                                                                                                                                        \
        UBENCH_DO_BENCHMARK()                                                                                                                                                                          \
        {                                                                                                                                                                                              \
            _base_rotatedata_mat( data, nrows, ncols, ridx1, ridx2 );                                                                                                                                  \
        }                                                                                                                                                                                              \
                                                                                                                                                                                                       \
        free( data );                                                                                                                                                                                  \
    }

#define BENCH_ROTATEDATA_MAT_RMAJ( NROWS, NCOLS, RIDX1, RIDX2 )                                                                                                                                        \
    UBENCH_EX( rotatedata_mat_rmaj, dim_##NROWS##x##NCOLS##_rowidx1_##RIDX1##_rowidx2_##RIDX2 )                                                                                                        \
    {                                                                                                                                                                                                  \
        base_int_t  nrows        = (base_int_t)NROWS;                                                                                                                                                  \
        base_int_t  ncols        = (base_int_t)NCOLS;                                                                                                                                                  \
        base_uint_t ridx1        = (base_int_t)RIDX1;                                                                                                                                                  \
        base_uint_t ridx2        = (base_int_t)RIDX2;                                                                                                                                                  \
        base_int_t  num_elements = nrows * ncols;                                                                                                                                                      \
        size_t      array_size   = (size_t)( num_elements * sizeof( double ) );                                                                                                                        \
        double     *data         = (double *)malloc( array_size );                                                                                                                                     \
                                                                                                                                                                                                       \
        set_mat_values( data, num_elements, ncols );                                                                                                                                                   \
                                                                                                                                                                                                       \
        UBENCH_DO_BENCHMARK()                                                                                                                                                                          \
        {                                                                                                                                                                                              \
            _base_rotatedata_mat_rmaj( data, nrows, ncols, ridx1, ridx2 );                                                                                                                             \
        }                                                                                                                                                                                              \
                                                                                                                                                                                                       \
        free( data );                                                                                                                                                                                  \
    }

// 3. Generate benchmarks for any dimensions (square or rectangular)
// 2^10
BENCH_ROTATEDATA_MAT( 1024, 1024, 0, 1 )
BENCH_ROTATEDATA_MAT_RMAJ( 1024, 1024, 0, 1 )
BENCH_ROTATEDATA_MAT( 1024, 1024, 0, 100 )
BENCH_ROTATEDATA_MAT_RMAJ( 1024, 1024, 0, 100 )
BENCH_ROTATEDATA_MAT( 1024, 1024, 0, 500 )
BENCH_ROTATEDATA_MAT_RMAJ( 1024, 1024, 0, 500 )
BENCH_ROTATEDATA_MAT( 1024, 1024, 0, 1023 )
BENCH_ROTATEDATA_MAT_RMAJ( 1024, 1024, 0, 1023 )
// 2^11
BENCH_ROTATEDATA_MAT( 1024, 2048, 0, 1 )
BENCH_ROTATEDATA_MAT_RMAJ( 1024, 2048, 0, 1 )
BENCH_ROTATEDATA_MAT( 1024, 2048, 0, 100 )
BENCH_ROTATEDATA_MAT_RMAJ( 1024, 2048, 0, 100 )
BENCH_ROTATEDATA_MAT( 1024, 2048, 0, 500 )
BENCH_ROTATEDATA_MAT_RMAJ( 1024, 2048, 0, 500 )
BENCH_ROTATEDATA_MAT( 1024, 2048, 0, 1023 )
BENCH_ROTATEDATA_MAT_RMAJ( 1024, 2048, 0, 1023 )
// // 2^12
// BENCH_ROTATEDATA_MAT(1024, 4096, 0, 1)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 4096, 0, 1)
// BENCH_ROTATEDATA_MAT(1024, 4096, 0, 100)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 4096, 0, 100)
// BENCH_ROTATEDATA_MAT(1024, 4096, 0, 500)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 4096, 0, 500)
// BENCH_ROTATEDATA_MAT(1024, 4096, 0, 1023)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 4096, 0, 1023)
// // 2^13
// BENCH_ROTATEDATA_MAT(1024, 8192, 0, 1)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 8192, 0, 1)
// BENCH_ROTATEDATA_MAT(1024, 8192, 0, 100)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 8192, 0, 100)
// BENCH_ROTATEDATA_MAT(1024, 8192, 0, 500)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 8192, 0, 500)
// BENCH_ROTATEDATA_MAT(1024, 8192, 0, 1023)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 8192, 0, 1023)
// // 2^14
// BENCH_ROTATEDATA_MAT(1024, 16384, 0, 1)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 16384, 0, 1)
// BENCH_ROTATEDATA_MAT(1024, 16384, 0, 100)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 16384, 0, 100)
// BENCH_ROTATEDATA_MAT(1024, 16384, 0, 500)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 16384, 0, 500)
// BENCH_ROTATEDATA_MAT(1024, 16384, 0, 1023)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 16384, 0, 1023)
// // 2^15
// BENCH_ROTATEDATA_MAT(1024, 32768, 0, 1)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 32768, 0, 1)
// BENCH_ROTATEDATA_MAT(1024, 32768, 0, 100)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 32768, 0, 100)
// BENCH_ROTATEDATA_MAT(1024, 32768, 0, 500)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 32768, 0, 500)
// BENCH_ROTATEDATA_MAT(1024, 32768, 0, 1023)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 32768, 0, 1023)
// // 2^16
// BENCH_ROTATEDATA_MAT(1024, 65536, 0, 1)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 65536, 0, 1)
// BENCH_ROTATEDATA_MAT(1024, 65536, 0, 100)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 65536, 0, 100)
// BENCH_ROTATEDATA_MAT(1024, 65536, 0, 500)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 65536, 0, 500)
// BENCH_ROTATEDATA_MAT(1024, 65536, 0, 1023)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 65536, 0, 1023)
// // 2^17
// BENCH_ROTATEDATA_MAT(1024, 131072, 0, 1)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 131072, 0, 1)
// BENCH_ROTATEDATA_MAT(1024, 131072, 0, 100)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 131072, 0, 100)
// BENCH_ROTATEDATA_MAT(1024, 131072, 0, 500)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 131072, 0, 500)
// BENCH_ROTATEDATA_MAT(1024, 131072, 0, 1023)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 131072, 0, 1023)
// // 2^18
// BENCH_ROTATEDATA_MAT(1024, 262144, 0, 1)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 262144, 0, 1)
// BENCH_ROTATEDATA_MAT(1024, 262144, 0, 100)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 262144, 0, 100)
// BENCH_ROTATEDATA_MAT(1024, 262144, 0, 500)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 262144, 0, 500)
// BENCH_ROTATEDATA_MAT(1024, 262144, 0, 1023)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 262144, 0, 1023)
// // 2^19
// BENCH_ROTATEDATA_MAT(1024, 524288, 0, 1)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 524288, 0, 1)
// BENCH_ROTATEDATA_MAT(1024, 524288, 0, 100)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 524288, 0, 100)
// BENCH_ROTATEDATA_MAT(1024, 524288, 0, 500)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 524288, 0, 500)
// BENCH_ROTATEDATA_MAT(1024, 524288, 0, 1023)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 524288, 0, 1023)
// // 2^20
// BENCH_ROTATEDATA_MAT(1024, 1048576, 0, 1)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 1048576, 0, 1)
// BENCH_ROTATEDATA_MAT(1024, 1048576, 0, 100)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 1048576, 0, 100)
// BENCH_ROTATEDATA_MAT(1024, 1048576, 0, 500)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 1048576, 0, 500)
// BENCH_ROTATEDATA_MAT(1024, 1048576, 0, 1023)
// BENCH_ROTATEDATA_MAT_RMAJ(1024, 1048576, 0, 1023)

// // 2^21
// BENCH_ROTATEDATA_MAT(10, 2097152, 0, 9)
// BENCH_ROTATEDATA_MAT_RMAJ(10, 2097152, 0, 9)
// // 2^22
// BENCH_ROTATEDATA_MAT(10, 4194304, 0, 9)
// BENCH_ROTATEDATA_MAT_RMAJ(10, 4194304, 0, 9)
// // 2^23
// BENCH_ROTATEDATA_MAT(10, 8388608, 0, 9)
// BENCH_ROTATEDATA_MAT_RMAJ(10, 8388608, 0, 9)
// // 2^24
// BENCH_ROTATEDATA_MAT(10, 16777216, 0, 9)
// BENCH_ROTATEDATA_MAT_RMAJ(10, 16777216, 0, 9)
// // 2^25
// BENCH_ROTATEDATA_MAT(10, 33554432, 0, 9)
// BENCH_ROTATEDATA_MAT_RMAJ(10, 33554432, 0, 9)
// // 2^26
// BENCH_ROTATEDATA_MAT(10, 67108864, 0, 9)
// BENCH_ROTATEDATA_MAT_RMAJ(10, 67108864, 0, 9)
// // 2^27
// BENCH_ROTATEDATA_MAT(10, 134217728, 0, 1)
// BENCH_ROTATEDATA_MAT_RMAJ(10, 134217728, 0, 1)
// // 2^28
// BENCH_ROTATEDATA_MAT(2, 268435456, 0, 1)
// BENCH_ROTATEDATA_MAT_RMAJ(2, 268435456, 0, 1)
// // 2^29
// BENCH_ROTATEDATA_MAT(2, 536870912, 0, 1)
// BENCH_ROTATEDATA_MAT_RMAJ(2, 536870912, 0, 1)
// //2^30
// BENCH_ROTATEDATA_MAT(2, 1073741824, 0, 1)
// BENCH_ROTATEDATA_MAT_RMAJ(2, 1073741824, 0, 1)

// 4. Generate the main() function
UBENCH_MAIN()

// int
// main()
// {
//     base_int_t ierr = 0;

//     base_int_t nrows = 2;
//     base_int_t ncols = 10;

//     base_int_t nele = nrows * ncols;

//     size_t mat_size = sizeof(double) * nele;

//     double *mat = (double*) malloc(mat_size);

//     set_mat_values(mat, nele, ncols);

//     free(mat);
//     mat = NULL;

//     return ierr;
// }