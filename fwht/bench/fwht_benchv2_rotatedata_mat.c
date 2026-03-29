#include "fwht.h"
#include "ubench.h"

// 1. Define your initialization helper function
void set_mat_values(double* mat, int nele, int ncols){
    for(int i = 0; i<nele; ++i){
        mat[i] = (((double)i) / ((double)ncols)) + 3.14748;
    }
}

// 2. Define the macro, replacing the loop with your function call
#define BENCH_ROTATEDATA_MAT(NROWS, NCOLS) \
    UBENCH_EX(rotatedata_mat, dim_##NROWS##x##NCOLS) { \
        base_int_t nrows = NROWS; \
        base_int_t ncols = NCOLS; \
        base_uint_t ridx1 = 0; \
        base_uint_t ridx2 = 1; \
        int num_elements = nrows * ncols; \
        double *data = (double *)malloc(num_elements * sizeof(double)); \
        \
        /* Call your initialization function here */ \
        set_mat_values(data, num_elements, ncols); \
        \
        /* ubench ONLY times the execution inside this block */ \
        UBENCH_DO_BENCHMARK() { \
            _base_rotatedata_mat(data, nrows, ncols, ridx1, ridx2); \
        } \
        \
        free(data); \
    }

#define BENCH_ROTATEDATA_MAT_RMAJ(NROWS, NCOLS) \
    UBENCH_EX(rotatedata_mat_rmaj, dim_##NROWS##x##NCOLS) { \
        base_int_t nrows = NROWS; \
        base_int_t ncols = NCOLS; \
        base_uint_t ridx1 = 0; \
        base_uint_t ridx2 = 1; \
        int num_elements = nrows * ncols; \
        double *data = (double *)malloc(num_elements * sizeof(double)); \
        \
        set_mat_values(data, num_elements, ncols); \
        \
        UBENCH_DO_BENCHMARK() { \
            _base_rotatedata_mat_rmaj(data, nrows, ncols, ridx1, ridx2); \
        } \
        \
        free(data); \
    }

// 3. Generate benchmarks for any dimensions (square or rectangular)
// 2^10
BENCH_ROTATEDATA_MAT(2, 1024)
BENCH_ROTATEDATA_MAT_RMAJ(2, 1024)
// 2^11
BENCH_ROTATEDATA_MAT(2, 2048)
BENCH_ROTATEDATA_MAT_RMAJ(2, 2048)
// 2^12
BENCH_ROTATEDATA_MAT(2, 4096)
BENCH_ROTATEDATA_MAT_RMAJ(2, 4096)
// 2^13
BENCH_ROTATEDATA_MAT(2, 8192)
BENCH_ROTATEDATA_MAT_RMAJ(2, 8192)
// 2^14
BENCH_ROTATEDATA_MAT(2, 16384)
BENCH_ROTATEDATA_MAT_RMAJ(2, 16384)
// 2^15
BENCH_ROTATEDATA_MAT(2, 32768)
BENCH_ROTATEDATA_MAT_RMAJ(2, 32768)
// 2^16
BENCH_ROTATEDATA_MAT(2, 65536)
BENCH_ROTATEDATA_MAT_RMAJ(2, 65536)
// 2^17
BENCH_ROTATEDATA_MAT(2, 131072)
BENCH_ROTATEDATA_MAT_RMAJ(2, 131072)
// 2^18
BENCH_ROTATEDATA_MAT(2, 262144)
BENCH_ROTATEDATA_MAT_RMAJ(2, 262144)
// 2^19
BENCH_ROTATEDATA_MAT(2, 524288)
BENCH_ROTATEDATA_MAT_RMAJ(2, 524288)
// 2^20 (RAM: 33 Mo)
BENCH_ROTATEDATA_MAT(2, 1048576)
BENCH_ROTATEDATA_MAT_RMAJ(2, 1048576)
// 2^21 (RAM: 67 Mo)
BENCH_ROTATEDATA_MAT(2, 2097152)
BENCH_ROTATEDATA_MAT_RMAJ(2, 2097152)
// 2^22 (RAM: 134 Mo)
BENCH_ROTATEDATA_MAT(2, 4194304)
BENCH_ROTATEDATA_MAT_RMAJ(2, 4194304)
// 2^23 (RAM: 268 Mo)
BENCH_ROTATEDATA_MAT(2, 8388608)
BENCH_ROTATEDATA_MAT_RMAJ(2, 8388608)
// 2^24 (RAM: 536 Mo)
BENCH_ROTATEDATA_MAT(2, 16777216)
BENCH_ROTATEDATA_MAT_RMAJ(2, 16777216)
// 2^25 (RAM: 1,07 Go)
BENCH_ROTATEDATA_MAT(2, 33554432)
BENCH_ROTATEDATA_MAT_RMAJ(2, 33554432)
// 2^26 (RAM: 2,14 Go)
BENCH_ROTATEDATA_MAT(2, 67108864)
BENCH_ROTATEDATA_MAT_RMAJ(2, 67108864)
// 2^27 (RAM: 4,29 Go)
BENCH_ROTATEDATA_MAT(2, 134217728)
BENCH_ROTATEDATA_MAT_RMAJ(2, 134217728)
// 2^28 (RAM: 8,58 Go)
BENCH_ROTATEDATA_MAT(2, 268435456)
BENCH_ROTATEDATA_MAT_RMAJ(2, 268435456)
// 2^29 (RAM: 17,17 Go)
BENCH_ROTATEDATA_MAT(2, 536870912)
BENCH_ROTATEDATA_MAT_RMAJ(2, 536870912)
// 2^30 (RAM: 34,35 Go)
//BENCH_ROTATEDATA_MAT(2, 1073741824)
//BENCH_ROTATEDATA_MAT_RMAJ(2, 1073741824)

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