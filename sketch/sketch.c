#include "sketch.h"

void
_base_mat_larnv( double *mat, int rank, base_int_t localsize, base_int_t ncols, base_int_t nb, int cite )
{
    int        ierr      = 0;
    base_int_t remainder = ncols % nb;
    base_int_t coef      = ( ncols - remainder ) / nb;
    if ( remainder > 0 )
        coef += 1;
    base_uint_t Rseed = 1234567;
    base_int_t  cur   = ( remainder == 0 ) ? nb : ( ( cite < coef - 1 ) ? nb : remainder );
    base_int_t *iseed = NULL;
    // iseed = (base_int_t*) mkl_malloc(sizeof(base_int_t)*4, base_ialign);
    SPEALLOC( iseed, base_ialign, sizeof( base_int_t ) * 4 );
    if ( iseed == NULL ) {
        printf( "base_mat_larnv::iseed couldn't be allocated (is NULL)\n" );
        abort();
    }
    srand( rank * Rseed + cite * Rseed );
    *( iseed + 0 ) = rand() % 4095;
    *( iseed + 1 ) = rand() % 4095;
    *( iseed + 2 ) = rand() % 4095;
    *( iseed + 3 ) = 5;
    ierr           = LAPACKE_dlarnv( 3, (int *)iseed, localsize * cur, mat );
    if ( ierr != 0 ) {
        printf( "_base_mat_larnv::LAPACKE_dlarnv::ierr::\n" );
        abort();
    }
    free( iseed );
    iseed = NULL;
}

void
_base_create_random_permutation( base_int_t input_len, base_int_t *data_out, base_int_t len_data_out )
{
    base_int_t  i, j, sw;
    base_int_t *rand_tmp = NULL;
    // rand_tmp             = (base_int_t *)malloc( sizeof( base_int_t ) * input_len );
    SPEALLOC( rand_tmp, base_ialign, sizeof( base_int_t ) * input_len );
    BASE_ASSERT_ISNOTNULL( rand_tmp );
    memset( rand_tmp, 0, sizeof( base_int_t ) * input_len );
    for ( i = 0; i < input_len; i++ )
        rand_tmp[i] = i;
    srand( 7654321 );
    for ( i = 0; i < len_data_out; i++ ) {
        j           = rand() % ( input_len - i ) + i;
        sw          = rand_tmp[i];
        rand_tmp[i] = rand_tmp[j];
        data_out[i] = rand_tmp[j];
        rand_tmp[j] = sw;
    }
    free( rand_tmp );
    rand_tmp = NULL;
}

void
base_init_sketch_data( base_sketch_t *sketch_data, base_int_t *iparam, int rank, int size )
{
    sketch_data->nrows_data_in      = iparam[IDX_NROWS_DATA_IN];
    sketch_data->ncols_data_in      = iparam[IDX_NCOLS_DATA_IN];
    sketch_data->sketch_dim         = iparam[IDX_SKETCH_DIM];
    sketch_data->nDr                = iparam[IDX_NDR];
    sketch_data->nDl                = iparam[IDX_NDL];
    sketch_data->sketch_alg         = iparam[IDX_SKETCH_ALG];
    sketch_data->sketch_num_threads = iparam[IDX_SKETCH_NUM_THREADS];
    sketch_data->sketch_num_gpu     = iparam[IDX_SKETCH_NUM_GPU];
    sketch_data->Hadaplan           = NULL;
    size_t      size_rademacher_array, size_permutation_array;  //, size_data_work;
    base_uint_t Rseed;
    base_int_t  i;
    switch ( sketch_data->sketch_alg ) {
        case GAUSS:
            sketch_data->sketch_type       = iparam[IDX_SKETCH_TYPE];  // not really used by GAUSS sketch
            sketch_data->rademacher_array  = NULL;
            sketch_data->permutation_array = NULL;
            sketch_data->nrows_data_work   = iparam[IDX_NROWS_DATA_IN];
            sketch_data->scale             = base_d_p1 / sqrt( (double)iparam[IDX_SKETCH_DIM] );
            break;
        case SRHT_HADI_FWHT:
        case SRHT_HADI_FWHT_OPENMP:
        case SRHT_HADI_FWHT_GPU:
        case SRHT_CFWHT:
        case SRHT_FFTW:
        default:
            sketch_data->sketch_type     = iparam[IDX_SKETCH_TYPE];
            sketch_data->nrows_data_work = base_n2p( iparam[IDX_NROWS_DATA_IN] );
            sketch_data->scale           = base_d_p1 / sqrt( (double)iparam[IDX_SKETCH_DIM] );  //(double) base_n2p(iparam[IDX_NROWS_DATA_IN])
            assert( base_ispow2( sketch_data->nrows_data_work ) );
            // Rademacher Vector
            if ( iparam[IDX_NDR] == 0 && iparam[IDX_NDL] == 0 ) { /**< .EQV. to identity vector */
                // sketch_data->rademacher_array = (base_int_t *)mkl_malloc( 0, base_ialign );
                SPEALLOC( sketch_data->rademacher_array, base_ialign, 0 );
                memset( sketch_data->rademacher_array, 0, 0 );
            }
            else { /**< Rademacher vector */
                sketch_data->nDr = 0;
                sketch_data->nDl = 0;
                Rseed            = 1234567 * rank + 1234567;
                srand( Rseed );
                for ( i = 0; i < sketch_data->nrows_data_in; ++i )
                    sketch_data->nDr += ( rand() % 2 == 0 ) ? 1 : 0; /**< Length of Rademacher right vector */
                if ( iparam[IDX_NDL] > 0 )
                    for ( i = 0; i < sketch_data->sketch_dim; ++i )
                        sketch_data->nDl += ( rand() % 2 == 0 ) ? 1 : 0; /**< Length of Rademacher left vector */
                size_rademacher_array = ( sketch_data->nDr + sketch_data->nDl ) * sizeof( base_int_t );
                // sketch_data->rademacher_array = (base_int_t *)mkl_malloc( size_rademacher_array, base_ialign );
                SPEALLOC( sketch_data->rademacher_array, base_ialign, size_rademacher_array );
                BASE_ASSERT_ISNOTNULL( ( sketch_data->rademacher_array ) );
                memset( sketch_data->rademacher_array, 0, size_rademacher_array );
                srand( Rseed );
                base_int_t i_tmp = 0;
                /**< Computing Rademacher Right vector */
                for ( i = 0; i < sketch_data->nrows_data_in; ++i ) {
                    if ( rand() % 2 == 0 ) {
                        sketch_data->rademacher_array[i_tmp] = i;
                        i_tmp += 1;
                    }
                }
                if ( iparam[IDX_NDL] > 0 ) {
                    /**< Computing RademacherLeft vector */
                    for ( i = 0; i < sketch_data->sketch_dim; ++i ) {
                        if ( rand() % 2 == 0 ) {
                            sketch_data->rademacher_array[i_tmp] = i;
                            i_tmp += 1;
                        }
                    }
                }
            }
            // Permutation Vector
            size_permutation_array = sketch_data->sketch_dim * sizeof( base_int_t );
            // sketch_data->permutation_array = (base_int_t *)mkl_malloc( size_permutation_array, base_ialign );
            SPEALLOC( sketch_data->permutation_array, base_ialign, size_permutation_array );
            BASE_ASSERT_ISNOTNULL( ( sketch_data->permutation_array ) );
            memset( sketch_data->permutation_array, 0, size_permutation_array );
            _base_create_random_permutation( sketch_data->nrows_data_in, sketch_data->permutation_array, sketch_data->sketch_dim );
            break;
    }
}

void
base_set_sketch_data( base_sketch_t *sketch_data, int rank, int size )
{
    // int         ierr = 0;
    // size_t      size_rademacher_array, size_permutation_array, size_data_work;
    // base_uint_t Rseed;
    // base_int_t  i;
    size_t size_data_work = 0;
    switch ( sketch_data->sketch_alg ) {
        case GAUSS:
            int ierr = 0;
            // Allocate Local Gaussian Matrix
            size_data_work = sketch_data->sketch_dim * sketch_data->nrows_data_work * sizeof( double );
            // sketch_data->data_work  = (double*) mkl_malloc(size_data_work, d_align);
            SPEALLOC( sketch_data->data_work, base_dalign, size_data_work );
            BASE_ASSERT_ISNOTNULL( ( sketch_data->data_work ) );
            memset( sketch_data->data_work, 0, size_data_work );
            // Create Local Gaussian Matrix
            //_base_MATgauss(sketch_data->data_work, rank, sketch_data->sketch_dim, sketch_data->nrows_data_work, sketch_data->nrows_data_work, 0);
            _base_mat_larnv( sketch_data->data_work, rank, sketch_data->sketch_dim, sketch_data->nrows_data_work, sketch_data->nrows_data_work, 0 );
            // Scale Local Gaussian Matrix
            ierr = LAPACKE_dlascl(
                LAPACK_COL_MAJOR, 'G', 0, 0, sqrt( (double)sketch_data->sketch_dim ), 1.0, sketch_data->sketch_dim, sketch_data->nrows_data_work, sketch_data->data_work, sketch_data->sketch_dim );
            if ( ierr != 0 ) {
                if ( !rank )
                    printf( "ierr = %d %s\n", ierr, "base_Set_dSKETCH::LAPACKE_dlascl::ierr" );
                abort();
            }
            /* base_memMB_SKETCH_d(&sketch_data->memMB, size_data_work);
            base_flops_SKETCH_d(sketch_data, size); */
            break;
        case SRHT_HADI_FWHT:
        case SRHT_HADI_FWHT_OPENMP:
        case SRHT_HADI_FWHT_GPU:
        case SRHT_CFWHT:
        case SRHT_FFTW:
        default:
            // Local srht buffer
            size_data_work = sketch_data->nrows_data_work * sketch_data->ncols_data_in * sizeof( double );
            // sketch_data->data_work = (double *)mkl_malloc( size_data_work, d_align );
            SPEALLOC( sketch_data->data_work, base_dalign, size_data_work );
            BASE_ASSERT_ISNOTNULL( ( sketch_data->data_work ) );
            if ( sketch_data->sketch_alg == SRHT_HADI_FWHT ) {
                fwht_config_t config              = { .backend = FWHT_BACKEND_CPU, .num_threads = 1, .gpu_device = 0, .normalize = true };
                sketch_data->sketch_hadi_fwht_ctx = fwht_create_context( &config );
            }
            else if ( sketch_data->sketch_alg == SRHT_HADI_FWHT_OPENMP ) {
                fwht_config_t config              = { .backend = FWHT_BACKEND_OPENMP, .num_threads = sketch_data->sketch_num_threads, .gpu_device = 0, .normalize = true };
                sketch_data->sketch_hadi_fwht_ctx = fwht_create_context( &config );
            }
            else if ( sketch_data->sketch_alg == SRHT_HADI_FWHT_GPU ) {
                fwht_config_t config              = { .backend = FWHT_BACKEND_GPU, .num_threads = 0, .gpu_device = sketch_data->sketch_num_gpu, .normalize = true };
                sketch_data->sketch_hadi_fwht_ctx = fwht_create_context( &config );
            }
            else if ( sketch_data->sketch_alg == SRHT_FFTW ) {
                view_t vIn;
                vIn.m   = sketch_data->nrows_data_work;
                vIn.n   = sketch_data->ncols_data_in;
                vIn.st1 = 1;
                vIn.st2 = sketch_data->nrows_data_work;
                if ( sketch_data->sketch_num_threads >= 1 ) {
                    base_SetFFTW_OMP( &sketch_data->Hadaplan, &vIn, sketch_data->sketch_num_threads, 1, sketch_data->data_work, sketch_data->data_work );
                }
                else {
                    base_SetFFTW( &sketch_data->Hadaplan, &vIn, 1, sketch_data->data_work, sketch_data->data_work );
                }
                BASE_ASSERT_ISNOTNULL( ( sketch_data->Hadaplan ) );
                // if (rank==0)fftw_print_plan(sketch_data->Hadaplan);
            }
            memset( sketch_data->data_work, 0, size_data_work );
            /* size_t size_rademacher_array  = ( sketch_data->nDr + sketch_data->nDl ) * sizeof( base_int_t );
            size_t size_permutation_array = sketch_data->sketch_dim * sizeof( base_int_t );
            base_memMB_SKETCH_d( &sketch_data->memMB, size_rademacher_array + size_permutation_array + size_data_work );
            base_flops_SKETCH_d( sketch_data, size ); */
            break;
    }
}

void
base_free_sketch_data( base_sketch_t *sketch_data )
{
    free( sketch_data->rademacher_array );
    sketch_data->rademacher_array = NULL;
    free( sketch_data->permutation_array );
    sketch_data->permutation_array = NULL;
    free( sketch_data->data_work );
    sketch_data->data_work = NULL;
    if ( sketch_data->sketch_alg == SRHT_FFTW ) {
        if ( sketch_data->sketch_num_threads >= 1 ) {
            base_FreeFFTW_OMP( &sketch_data->Hadaplan );
        }
        else {
            base_FreeFFTW( &sketch_data->Hadaplan );
        }
    }
    else if ( sketch_data->sketch_alg == SRHT_HADI_FWHT || sketch_data->sketch_alg == SRHT_HADI_FWHT_OPENMP || sketch_data->sketch_alg == SRHT_HADI_FWHT_GPU ) {
        fwht_destroy_context( sketch_data->sketch_hadi_fwht_ctx );
    }
}

void
base_getdata_sketch_data( base_sketch_t *sketch_data, const base_int_t ncols_data_in, double *data_in, double *data_out )
{
    assert( !( ncols_data_in > sketch_data->ncols_data_in ) );  // ncols_data_in cannot be bigger than the previous one declared with base_Set_sketch_data !
    sketch_data->ncols_data_in = ncols_data_in;
    sketch_data->data_in       = data_in;
    sketch_data->data_out      = data_out;
}

// SKETCH functions
void
base_compute_sketch( base_sketch_t *sketch_data )
{
    base_int_t i = 0;
    // Renaming
    base_int_t  nrows_data_in     = sketch_data->nrows_data_in;
    base_int_t  sketch_dim        = sketch_data->sketch_dim;
    base_int_t  nD                = sketch_data->nDr;
    base_int_t  nrows_data_work   = sketch_data->nrows_data_work;
    base_int_t *rademacher_array  = sketch_data->rademacher_array;
    base_int_t *permutation_array = sketch_data->permutation_array;
    double      scale             = sketch_data->scale;
    double     *data_in           = sketch_data->data_in;
    double     *data_out          = sketch_data->data_out;
    double     *data_work         = sketch_data->data_work;
    switch ( sketch_data->sketch_alg ) {
        case GAUSS:
            cblas_dgemm( CblasColMajor,
                         CblasNoTrans,
                         CblasNoTrans,
                         sketch_dim,
                         1,
                         sketch_data->nrows_data_work,
                         base_d_alpha_p1,
                         sketch_data->data_work,
                         sketch_dim,
                         sketch_data->data_in,
                         sketch_data->nrows_data_work,
                         base_d_beta_ze,
                         data_out,
                         sketch_dim );
            break;
        case SRHT_HADI_FWHT:
        case SRHT_HADI_FWHT_OPENMP:
        case SRHT_HADI_FWHT_GPU:
        case SRHT_CFWHT:
        case SRHT_FFTW:
        default:
            // Prepare Data
            memset( data_work + nrows_data_in, 0, sizeof( double ) * ( nrows_data_work - nrows_data_in ) );
            cblas_dcopy( nrows_data_in, data_in, 1, data_work, 1 );
            // Rademacher multiplication (Optim:: Maybe try search for blas function to do it)
            for ( i = 0; i < nD; ++i )
                data_work[rademacher_array[i]] *= -1;
            // FWHT computation
            switch ( sketch_data->sketch_alg ) {
                case SRHT_HADI_FWHT:
                case SRHT_HADI_FWHT_OPENMP:
                case SRHT_HADI_FWHT_GPU:
                    fwht_status_t status = fwht_batch_f64_contiguous( sketch_data->sketch_hadi_fwht_ctx, data_work, nrows_data_work, 1 );
                    if ( status != FWHT_SUCCESS ) {
                        fprintf( stderr, "%s\n", fwht_error_string( status ) );
                    }
                    break;
                case SRHT_FFTW:
                    fftw_execute_r2r( sketch_data->Hadaplan, data_work, data_work );
                    break;
                case SRHT_CFWHT:
                default:
                    base_fwht_mat_v3( data_work, NULL, nrows_data_work, 1 );
                    break;
            }
            // Random sampling
            for ( i = 0; i < sketch_dim; ++i )
                cblas_dcopy( 1, &data_work[permutation_array[i]], 1, &data_out[i], 1 );
            // Scaling
            cblas_dscal( sketch_dim, scale, data_out, 1 );
            break;
    }
}

void
base_compute_sketch_mat( base_sketch_t *sketch_data )
{
    int        ierr = 0;
    base_int_t i    = 0;
    base_int_t j    = 0;
    // Renaming
    base_int_t  nrows_data_in     = sketch_data->nrows_data_in;
    base_int_t  ncols_data_in     = sketch_data->ncols_data_in;
    base_int_t  sketch_dim        = sketch_data->sketch_dim;
    base_int_t  nD                = sketch_data->nDr;
    base_int_t  nrows_data_work   = sketch_data->nrows_data_work;
    base_int_t *rademacher_array  = sketch_data->rademacher_array;
    base_int_t *permutation_array = sketch_data->permutation_array;
    double      scale             = sketch_data->scale;
    double     *data_in           = sketch_data->data_in;
    double     *data_out          = sketch_data->data_out;
    double     *data_work         = sketch_data->data_work;
    switch ( sketch_data->sketch_alg ) {
        case GAUSS:
            cblas_dgemm( CblasColMajor,
                         CblasNoTrans,
                         CblasNoTrans,
                         sketch_dim,
                         ncols_data_in,
                         sketch_data->nrows_data_work,
                         base_d_alpha_p1,
                         sketch_data->data_work,
                         sketch_dim,
                         sketch_data->data_in,
                         sketch_data->nrows_data_work,
                         base_d_beta_ze,
                         data_out,
                         sketch_dim );
            break;
        case SRHT_HADI_FWHT:
        case SRHT_HADI_FWHT_OPENMP:
        case SRHT_HADI_FWHT_GPU:
        case SRHT_CFWHT:
        case SRHT_FFTW:
        default:
            // Prepare Data
            for ( j = 0; j < ncols_data_in; ++j ) {
                memset( data_work + ( j * nrows_data_work + nrows_data_in ), 0, sizeof( double ) * ( nrows_data_work - nrows_data_in ) );
            }
            //PRINT_COLMAJ_MAT(data_work, nrows_data_work, ncols_data_in , "data_work");
            // memset(data_work, 0, sizeof(double)*nrows_data_work*ncols_data_in);
            ierr = LAPACKE_dlacpy( LAPACK_COL_MAJOR, 'A', nrows_data_in, ncols_data_in, data_in, nrows_data_in, data_work, nrows_data_work );
            if ( ierr != 0 ) {
                printf( "base_compute_sketch_mat::LAPACKE_dlacpy::ierr != 0\n" );
                abort();
            }
            // Rademacher multiplication (Optim:: Maybe try search for blas function to do it)
            for ( j = 0; j < ncols_data_in; ++j ) {
                for ( i = 0; i < nD; ++i )
                    data_work[j * nrows_data_work + rademacher_array[i]] *= -1;
            }
            // FWHT computation
            switch ( sketch_data->sketch_alg ) {
                case SRHT_HADI_FWHT:
                case SRHT_HADI_FWHT_OPENMP:
                case SRHT_HADI_FWHT_GPU:
                    fwht_status_t status = fwht_batch_f64_contiguous( sketch_data->sketch_hadi_fwht_ctx, data_work, nrows_data_work, ncols_data_in );
                    if ( status != FWHT_SUCCESS ) {
                        fprintf( stderr, "%s\n", fwht_error_string( status ) );
                    }
                    break;
                case SRHT_FFTW:
                    fftw_execute_r2r( sketch_data->Hadaplan, data_work, data_work );
                    break;
                case SRHT_CFWHT:
                default:
                    base_fwht_mat_v3( data_work, NULL, nrows_data_work, ncols_data_in );
                    break;
            }
            // Random sampling
            for ( j = 0; j < ncols_data_in; ++j ) {
                for ( i = 0; i < sketch_dim; ++i )
                    cblas_dcopy( 1, &data_work[j * nrows_data_work + permutation_array[i]], 1, &data_out[j * sketch_dim + i], 1 );
            }
            // Scaling (Optim:: Maybe put this step in base_BLOCK_SKETCH_d after MPI_Allreduce)
            cblas_dscal( sketch_dim * ncols_data_in, scale, data_out, 1 );
            break;
    }
}

void
base_compute_block_sketch_nocomm( base_sketch_t *sketch_data )
{
    //int         ierr = 0;
    base_int_t  i;
    base_int_t  j;
    base_int_t  ncols_data_in    = sketch_data->ncols_data_in;
    base_int_t  sketch_dim       = sketch_data->sketch_dim;
    base_int_t  nDr              = sketch_data->nDr;
    base_int_t  nDl              = sketch_data->nDl;
    base_int_t *rademacher_array = sketch_data->rademacher_array;
    double     *data_out         = sketch_data->data_out;

    switch ( sketch_data->sketch_alg ) {
        case GAUSS:
            cblas_dgemm( CblasColMajor,
                         CblasNoTrans,
                         CblasNoTrans,
                         sketch_dim,
                         ncols_data_in,
                         sketch_data->nrows_data_work,
                         base_d_alpha_p1,
                         sketch_data->data_work,
                         sketch_dim,
                         sketch_data->data_in,
                         sketch_data->nrows_data_work,
                         base_d_beta_ze,
                         data_out,
                         sketch_dim );
            break;
        case SRHT_HADI_FWHT:
        case SRHT_HADI_FWHT_OPENMP:
        case SRHT_HADI_FWHT_GPU:
        case SRHT_CFWHT:
        case SRHT_FFTW:
        default:
            // SRHT
            if ( sketch_data->sketch_type == SKETCH_1D ) {  // sequential np = 1 and ncols_data_in = 1 (np is number of MPI process)
                base_compute_sketch( sketch_data );
            }
            else if ( sketch_data->sketch_type == SKETCH_2D ) {  // parallel np >= 1 and ncols_data_in >= 1
                base_compute_sketch_mat( sketch_data );
            }
            // Rademacher Left vector product
            if ( sketch_data->nDl > 0 ) {
                for ( j = 0; j < ncols_data_in; ++j ) {
                    for ( i = 0; i < nDl; ++i )
                        data_out[j * sketch_dim + rademacher_array[nDr + i]] *= -1;
                }
            }
            break;
    }
}