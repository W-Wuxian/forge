#include "fwht.h"

void set_mat_values(double* mat, int nele, int ncols){
    for(int i = 0; i<nele; ++i){
        mat[i] = (((double)i) / ((double)ncols)) + 3.14748;
    }
}

int
main()
{
    base_int_t ierr = 0;

    base_int_t nrows = 2;
    base_int_t ncols = 10;

    base_int_t nele = nrows * ncols;

    size_t mat_size = sizeof(double) * nele;

    double *mat = (double*) malloc(mat_size);

    set_mat_values(mat, nele, ncols);

    

    free(mat);
    mat = NULL;

    return ierr;
}