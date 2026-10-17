#include "rgs.h"

void
base_set_rgs( rgs_data_t *rgs_data, base_int_t *iparam, int rank, int size, MPI_Comm comm )
{
    // Set MPI info
    rgs_data->rank = rank;
    rgs_data->size = size;
    rgs_data->comm = comm;
    // Set RGS dims
    rgs_data->nrows_data_in = iparam[IDX_NROWS_DATA_IN];
    rgs_data->ncols_data_in = iparam[IDX_NCOLS_DATA_IN];
    rgs_data->sketch_dim    = iparam[IDX_SKETCH_DIM];
    // Set Least-Squares Solver Flag
    rgs_data->lss_alg            = iparam[IDX_LSS_ALG];
    rgs_data->number_reortho_lss = iparam[IDX_NUMBER_REORTHO_LSS];
    // Set Inter-Block Orthogonalization
    rgs_data->number_reortho_sketched_cholqr = iparam[IDX_NUMBER_REORTHO_SKETCHED_CHOLQR];
    // Init current RGS iteration
    rgs_data->current_rgs_ite = 0;
    // Set SKETCH dims
    rgs_data->user_block_size      = iparam[IDX_USER_BLOCK_SIZE];
    rgs_data->remaining_block_size = iparam[IDX_NCOLS_DATA_IN] % iparam[IDX_USER_BLOCK_SIZE];
    rgs_data->number_of_block      = ( iparam[IDX_NCOLS_DATA_IN] - rgs_data->remaining_block_size ) / iparam[IDX_USER_BLOCK_SIZE];
    if ( rgs_data->remaining_block_size > 0 )
        rgs_data->number_of_block += 1;
    rgs_data->current_blocks_size                  = ( iparam[IDX_USER_BLOCK_SIZE] > rgs_data->remaining_block_size ) ? iparam[IDX_USER_BLOCK_SIZE] : rgs_data->remaining_block_size;
    base_int_t SKETCH_iparam[SKETCH_iparam_length] = { iparam[IDX_NROWS_DATA_IN], rgs_data->current_blocks_size, iparam[IDX_SKETCH_DIM], iparam[NDR], iparam[NDL], iparam[WSKETCH] };
    // Init SKETCH
    base_init_sketch_data( &rgs_data->sketch_data, SKETCH_iparam, rank, size );
    // Set mem
    base_int_t size_tmpwork = ( rgs_data->sketch_dim * rgs_data->current_blocks_size );
    base_int_t Stmp         = ( rgs_data->sketch_dim * rgs_data->current_blocks_size * rgs_data->number_of_block );
    base_int_t worktmp      = ( 2 * rgs_data->ncols_data_in * rgs_data->ncols_data_in ) * ( size + 1 );
    base_int_t addtmp       = base_imax( Stmp, worktmp );
    rgs_data->lth_tmpwork   = addtmp;
    size_tmpwork += addtmp;
    rgs_data->si_tmpwork    = size_tmpwork;
    base_int_t size_PSke    = rgs_data->sketch_dim * rgs_data->ncols_data_in;
    base_int_t size_SSke    = rgs_data->sketch_dim * rgs_data->ncols_data_in;
    base_int_t size_Rfac    = rgs_data->ncols_data_in * rgs_data->ncols_data_in;
    base_int_t size_Qfac    = rgs_data->nrows_data_in * rgs_data->ncols_data_in;
    base_int_t size_tmpTSQR = rgs_data->nrows_data_in * rgs_data->user_block_size;

    base_int_t size_workd = size_tmpwork + size_PSke + size_SSke + size_tmpTSQR;
    size_workd += ( size_Rfac + size_Qfac );
    rgs_data->size_workd = size_workd;
    rgs_data->data_work  = (double *)mkl_malloc( size_workd * sizeof( double ), d_align );
    BASE_ASSERT_ISNOTNULL( rgs_data->data_work );
    // Set memory to Zero
    memset( rgs_data->data_work, 0, size_workd * sizeof( double ) );
    // Set ptr
    base_int_t offset = 0;
    rgs_data->tmpwork = rgs_data->data_work;
    offset += size_tmpwork;
    rgs_data->PSke = rgs_data->data_work + offset;
    offset += size_PSke;
    rgs_data->SSke = rgs_data->data_work + offset;
    offset += size_SSke;
    rgs_data->Rfac = rgs_data->data_work + offset;
    offset += size_Rfac;
    rgs_data->Qfac = rgs_data->data_work + offset;
    offset += size_Qfac;
    rgs_data->tmpTSQR = rgs_data->data_work + offset;
    offset += size_tmpTSQR;
    // Set Sketch
    base_Set_d_SKETCH( &rgs_data->sketch_data, rank, size );
    BASE_ASSERT_ISNOTNULL( ( rgs_data->sketch_data.swork != NULL ) );

    // Approx memory used and flops
    base_int_t sum = ( size_PSke + size_SSke + size_Qfac + size_tmpTSQR + size_Rfac + size_tmpwork );
    base_memMB_RGS_d( &rgs_data->memMB, rgs_data->sketch_data.memMB, sum * sizeof( double ) );
    base_flops_RGS_d( rgs_data );
}