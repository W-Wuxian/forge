#include "sketch.h"

void
_base_create_random_permutation( base_int_t input_len, base_int_t *data_out, base_int_t len_data_out )
{
    base_int_t  i, j, sw;
    base_int_t *rand_tmp = NULL;
    rand_tmp             = (base_int_t *)malloc( sizeof( base_int_t ) * input_len );
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

void base_Init_sketch_data(base_sketch_t* sketch_data, base_int_t* iparam, int rank, int size){
  sketch_data->nrows_data_in      = iparam[NIN];
  sketch_data->ncols_data_in    = iparam[NCOLS];
  sketch_data->sketch_dim    = iparam[KPROJ];
  sketch_data->nDr      = iparam[NDR];
  sketch_data->nDl      = iparam[NDL];
  sketch_data->sketch_alg  = iparam[WSKETCH];
  sketch_data->Hadaplan = NULL;
  size_t size_rademacher_array, size_permutation_array, size_data_work;
  base_uint_t Rseed;
  base_int_t i;
  switch (sketch_data->sketch_alg)
  {
  case GAUSS:
    sketch_data->rademacher_array      = NULL;
    sketch_data->permutation_array   = NULL;
    sketch_data->nrows_data_work = iparam[NIN];
    sketch_data->scale  = dbase_p1 / sqrt((double) iparam[KPROJ]);
    break;
  case SRHT_CFWHT:
  case SRHT_FFTW:
  default:
    sketch_data->nrows_data_work   = base_N2P(iparam[NIN]);
    sketch_data->scale    =  dbase_p1 / sqrt((double) iparam[KPROJ]); //(double) base_N2P(iparam[NIN])
    CPLM_ASSERT(base_ISPOW2(sketch_data->nrows_data_work));
    // Rademacher Vector
    if (iparam[NDR] == 0 && iparam[NDL] == 0){ /**< .EQV. to identity vector */
      sketch_data->rademacher_array = (base_int_t*) mkl_malloc(0, base_ialign);
      memset(sketch_data->rademacher_array, 0, 0);
    }else{ /**< Rademacher vector */
      sketch_data->nDr = 0;
      sketch_data->nDl = 0;
      Rseed       = 1234567 * rank + 1234567;
      srand(Rseed);
      for (i = 0; i < sketch_data->nrows_data_in; ++i)sketch_data->nDr += (rand()%2==0) ? 1 : 0; /**< Length of Rademacher right vector */
      if (iparam[NDL]>0)for (i = 0; i < sketch_data->sketch_dim; ++i)sketch_data->nDl += (rand()%2==0) ? 1 : 0; /**< Length of Rademacher left vector */
      size_rademacher_array = (sketch_data->nDr + sketch_data->nDl) * sizeof(base_int_t);
      sketch_data->rademacher_array = (base_int_t*) mkl_malloc(size_rademacher_array, base_ialign);
      CPLM_ASSERT((sketch_data->rademacher_array!=NULL));
      memset(sketch_data->rademacher_array, 0, size_rademacher_array);
      srand(Rseed);
      base_int_t i_tmp = 0;
      /**< Computing Rademacher Right vector */
      for (i = 0; i < sketch_data->nrows_data_in; ++i){
        if (rand()%2 == 0){
          sketch_data->rademacher_array[i_tmp] = i;
          i_tmp += 1;
        }
      }
      if (iparam[NDL]>0){
        /**< Computing RademacherLeft vector */
        for (i = 0; i < sketch_data->sketch_dim; ++i){
          if (rand()%2 == 0){
            sketch_data->rademacher_array[i_tmp] = i;
            i_tmp += 1;
          }
        }
      }
    }
    // Permutation Vector
    size_permutation_array    = sketch_data->sketch_dim * sizeof(base_int_t);
    sketch_data->permutation_array = (base_int_t*) mkl_malloc(size_permutation_array, base_ialign);
    CPLM_ASSERT((sketch_data->permutation_array!=NULL));
    memset(sketch_data->permutation_array, 0, size_permutation_array);
    _base_CreateRandPerm(sketch_data->nrows_data_in, sketch_data->permutation_array, sketch_data->sketch_dim);
    break;
  }
}

void base_Set_sketch_data(base_sketch_t* sketch_data, int rank, int size){
  int ierr = 0;
  size_t size_rademacher_array, size_permutation_array, size_data_work;
  base_uint_t Rseed;
  base_int_t i;
  switch (sketch_data->sketch_alg)
  {
  case GAUSS:
    // Allocate Local Gaussian Matrix
    size_data_work = sketch_data->sketch_dim * sketch_data->nrows_data_work * sizeof(double);
    sketch_data->data_work  = (double*) mkl_malloc(size_data_work, d_align);
    CPLM_ASSERT((sketch_data->data_work!=NULL));
    memset(sketch_data->data_work, 0, size_data_work);
    // Create Local Gaussian Matrix
    //_base_MATgauss(sketch_data->data_work, rank, sketch_data->sketch_dim, sketch_data->nrows_data_work, sketch_data->nrows_data_work, 0);
    _base_MATlarnv_d(sketch_data->data_work, rank, sketch_data->sketch_dim, sketch_data->nrows_data_work, sketch_data->nrows_data_work, 0);
    // Scale Local Gaussian Matrix
    ierr = LAPACKE_dlascl(LAPACK_COL_MAJOR,'G',0,0,sqrt((double) sketch_data->sketch_dim),1.0,sketch_data->sketch_dim,sketch_data->nrows_data_work,sketch_data->data_work,sketch_data->sketch_dim);
    if (ierr != 0){
      if (!rank) printf("ierr = %d\n", ierr);
      CPLM_Abort("base_Set_dSKETCH::LAPACKE_dlascl::ierr");
    }
    base_memMB_SKETCH_d(&sketch_data->memMB, size_data_work);
    base_flops_SKETCH_d(sketch_data, size);
    break;
  case SRHT_CFWHT:
  case SRHT_FFTW:
  default:
    // Local srht buffer
    size_data_work = sketch_data->nrows_data_work * sketch_data->ncols_data_in * sizeof(double);
    sketch_data->data_work = (double*) mkl_malloc(size_data_work, d_align);
    CPLM_ASSERT((sketch_data->data_work!=NULL));
    if(sketch_data->sketch_alg == SRHT_FFTW){
      view_t vIn;
      vIn.m   = sketch_data->nrows_data_work;
      vIn.n   = sketch_data->ncols_data_in;
      vIn.st1 = 1;
      vIn.st2 = sketch_data->nrows_data_work;
      base_SetFFTW_d(&sketch_data->Hadaplan, &vIn, 1, sketch_data->data_work, sketch_data->data_work);
      CPLM_ASSERT((sketch_data->Hadaplan!=NULL));
      //if (rank==0)fftw_print_plan(sketch_data->Hadaplan);
    }
    memset(sketch_data->data_work, 0, size_data_work);
    size_rademacher_array = (sketch_data->nDr + sketch_data->nDl) * sizeof(base_int_t);
    size_permutation_array = sketch_data->sketch_dim * sizeof(base_int_t);
    base_memMB_SKETCH_d(&sketch_data->memMB, size_rademacher_array+size_permutation_array+size_data_work);
    base_flops_SKETCH_d(sketch_data, size);
    break;
  }
}

void base_Free_sketch_data(base_sketch_t* sketch_data){
  mkl_free(sketch_data->rademacher_array);
  sketch_data->rademacher_array = NULL;
  mkl_free(sketch_data->permutation_array);
  sketch_data->permutation_array = NULL;
  mkl_free(sketch_data->data_work);
  sketch_data->data_work = NULL;
  if(sketch_data->sketch_alg==SRHT_FFTW)base_FreeFFTW_d(&sketch_data->Hadaplan);
}


void base_GetData_sketch_data(base_sketch_t* sketch_data, const base_int_t ncols_data_in, double* In, double* Out){
  CPLM_ASSERT(!(ncols_data_in > sketch_data->ncols_data_in));// ncols_data_in cannot be bigger than the previous one declared with base_Set_sketch_data !
  sketch_data->ncols_data_in = ncols_data_in;
  sketch_data->In  = In;
  sketch_data->Out = Out;
}



// SKETCH functions
void base_SKETCH_d(base_sketch_t* sketch_data) {
  base_int_t i = 0;
  // Renaming
  base_int_t nrows_data_in    = sketch_data->nrows_data_in;
  base_int_t sketch_dim  = sketch_data->sketch_dim;
  base_int_t nD     = sketch_data->nDr;
  base_int_t nrows_data_work = sketch_data->nrows_data_work;
  base_int_t* rademacher_array     = sketch_data->rademacher_array;
  base_int_t* permutation_array  = sketch_data->permutation_array;
  double scale  = sketch_data->scale;
  double* In    = sketch_data->In;
  double* Out   = sketch_data->Out;
  double* data_work = sketch_data->data_work;
  switch (sketch_data->sketch_alg)
  {
  case GAUSS:
    cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, sketch_dim, 1, sketch_data->nrows_data_work, dbase_alpha_p1,
     sketch_data->data_work, sketch_dim, sketch_data->In, sketch_data->nrows_data_work, dbase_beta_ze, Out, sketch_dim);
    break;
  case SRHT_CFWHT:
  case SRHT_FFTW:
  default:
    // Prepare Data
    memset(data_work+nrows_data_in, 0, sizeof(double)*(nrows_data_work-nrows_data_in));
    cblas_dcopy(nrows_data_in, In, 1, data_work, 1);
    // Rademacher multiplication (Optim:: Maybe try search for blas function to do it)
    for (i = 0; i < nD; ++i)data_work[rademacher_array[i]] *= -1;
    // FWHT computation
    switch (sketch_data->sketch_alg)
    {
    case SRHT_FFTW:
      fftw_execute_r2r(sketch_data->Hadaplan, data_work, data_work); 
      break;
    case SRHT_CFWHT:
    default:
      base_FWHT_d(data_work, nrows_data_work);
      break;
    }
    // Random sampling
    for(i = 0; i < sketch_dim; ++i)cblas_dcopy(1, &data_work[permutation_array[i]], 1, &Out[i], 1);
    // Scaling
    cblas_dscal(sketch_dim, scale, Out, 1);
    break;
  }
}