#include "sketch.h"

int
main()
{
    int        ierr         = 0;
    base_int_t sketch_iparam[IDX_SKETCH_IPARAM_LEN];
    base_int_t len_In = 0;
    base_int_t len_Out = 0;
    base_sketch_t sketch_data;
    // Data to be transformed (Out-of-place)
    double* In    = NULL;
    // The transformed transformed
    double* Out   = NULL;

    sketch_iparam[IDX_NCOLS_DATA_IN] = 1;
    sketch_iparam[IDX_NROWS_DATA_IN] = 8;
    sketch_iparam[IDX_SKETCH_DIM] = 4;
    sketch_iparam[IDX_NDL] = 1;
    sketch_iparam[IDX_NDR] = 1;
    sketch_iparam[IDX_SKETCH_ALG] = SRHT_FFTW;
    //sketch_iparam[IDX_SKETCH_ALG] = SRHT_HADI_FWHT;

    len_In = sketch_iparam[IDX_NROWS_DATA_IN] * sketch_iparam[IDX_NCOLS_DATA_IN];
    len_Out = sketch_iparam[IDX_SKETCH_DIM] * sketch_iparam[IDX_NCOLS_DATA_IN];

    size_t size_In = len_In * sizeof(double);
    //In = (double*) mkl_malloc(size_In, d_align);
    SPEALLOC(In, base_dalign, size_In);
    assert(In!=NULL);
    memset(In, 0, size_In);
    size_t size_Out = len_Out * sizeof(double);
    //Out = (double*) mkl_malloc(size_Out, d_align);
    SPEALLOC(Out, base_dalign, size_Out);
    assert(Out!=NULL);
    memset(In, 0, size_In);

    base_init_sketch_data( &sketch_data, &sketch_iparam[0], 0, 1 );
    base_set_sketch_data( &sketch_data, 0, 1 );

    for ( base_int_t i = 0; i < len_In; ++i )In[i] = (double)i;

    PRINT_ARRAY( In, "%lf\n", len_In, "In Init" );
    PRINT_ARRAY( Out, "%lf\n", len_Out, "Out Init" );

    base_getdata_sketch_data(&sketch_data, sketch_iparam[IDX_NCOLS_DATA_IN], In, Out);

    base_compute_sketch( &sketch_data );
    
    PRINT_ARRAY( In, "%lf\n", len_In, "In" );
    PRINT_ARRAY( Out, "%lf\n", len_Out, "Out Computed" );

    base_free_sketch_data( &sketch_data );
    free(In);In=NULL;
    free(Out);Out=NULL;
    
    return ierr;
}