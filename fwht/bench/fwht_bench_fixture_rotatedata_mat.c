#include "fwht.h"
#include "ubench.h"

void
set_mat_values( double *mat, int nele, int ncols )
{
    for ( int i = 0; i < nele; ++i ) {
        mat[i] = ( ( (double)i ) / ( (double)ncols ) ) + 3.14748;
    }
}
// 1. Declare a struct that holds the state for your benchmark
struct fwht_fixture {
    double     *data;
    base_int_t  nrows;
    base_int_t  ncols;
    base_uint_t ridx1;
    base_uint_t ridx2;
};

// 2. Setup the state (called once before the benchmark timing starts)
UBENCH_F_SETUP( fwht_fixture )
{
    ubench_fixture->nrows = 2;
    ubench_fixture->ncols = 1024;
    ubench_fixture->ridx1 = 0;
    ubench_fixture->ridx2 = 1;

    base_int_t num_elements = ubench_fixture->nrows * ubench_fixture->ncols;
    size_t     data_size    = num_elements * sizeof( double );
    ubench_fixture->data    = (double *)malloc( data_size );

    // Initialize with dummy data so the compiler doesn't optimize it away
    set_mat_values( ubench_fixture->data, num_elements, ubench_fixture->ncols );
}

// 3. Teardown the state (called once after the benchmark finishes)
UBENCH_F_TEARDOWN( fwht_fixture )
{
    free( ubench_fixture->data );
}

// 4. Define the benchmark for the standard rotation
UBENCH_F( fwht_fixture, rotatedata_mat )
{
    _base_rotatedata_mat( ubench_fixture->data, ubench_fixture->nrows, ubench_fixture->ncols, ubench_fixture->ridx1, ubench_fixture->ridx2 );
}

// 5. Define the benchmark for the row-major rotation
UBENCH_F( fwht_fixture, rotatedata_mat_rmaj )
{
    _base_rotatedata_mat_rmaj( ubench_fixture->data, ubench_fixture->nrows, ubench_fixture->ncols, ubench_fixture->ridx1, ubench_fixture->ridx2 );
}

// 6. Generate the main() function
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