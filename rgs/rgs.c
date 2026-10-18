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
    if ( rgs_data->remaining_block_size > 0 ) {
        rgs_data->number_of_block += 1;
    }
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

    base_int_t size_data_work = size_tmpwork + size_PSke + size_SSke + size_tmpTSQR;
    size_data_work += ( size_Rfac + size_Qfac );
    rgs_data->size_data_work = size_data_work;
    SPEALLOC( rgs_data->data_work, base_dalign, size_data_work * sizeof( double ) );
    BASE_ASSERT_ISNOTNULL( rgs_data->data_work );
    // Set memory to Zero
    memset( rgs_data->data_work, 0, size_data_work * sizeof( double ) );
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
    base_set_sketch_data( &rgs_data->sketch_data, rank, size );
    BASE_ASSERT_ISNOTNULL( ( rgs_data->sketch_data.swork != NULL ) );

    // Approx memory used and flops
    base_int_t sum = ( size_PSke + size_SSke + size_Qfac + size_tmpTSQR + size_Rfac + size_tmpwork );
    base_memory_rgs( &rgs_data->rgs_memory, rgs_data->sketch_data.sketch_memory, (size_t)( sum * sizeof( double ) ) );
    base_flops_rgs( rgs_data );
}

void
base_free_rgs_data( base_rgs_t *rgs_data )
{
    base_free_sketch_data( &rgs_data->sketch_data );
    SPEFREE( rgs_data->data_work );
    rgs_data->tmp_data_work = NULL;
    rgs_data->pske          = NULL;
    rgs_data->sske          = NULL;
    rgs_data->rfac          = NULL;
    rgs_data->qfac          = NULL;
    rgs_data->tmp_data_tsqr = NULL;
}

void
base_flops_rgs( base_rgs_t *rgs_data )
{
    double coef = rgs_data->number_of_block;
    double m    = rgs_data->nrows_data_in;
    double bs   = rgs_data->current_blocks_size;
    double nb   = rgs_data->user_block_size;
    double k    = rgs_data->sketch_dim;

    double fp_LSS       = 0;
    double sumi         = ( coef + 1 ) * ( coef / 2 );
    double sumi2        = ( coef * coef * coef / 3 ) - ( coef * coef / 2 ) + ( coef / 6 );
    double sumi3        = ( coef / 2 ) * ( coef + 1 );
    double fp_Qupd      = 2 * m * bs * ( 1 + nb * sumi );
    double fp_SKETCH    = (double)rgs_data->sketch_data.sketch_flops;
    double fp_SCHOLQR   = ( bs == 1 ) ? 0 : fp_SKETCH + 2 * bs * bs * k + ( bs * bs * bs / 3 ) + ( bs * bs / 2 ) + m * m * bs;
    double fp_L2SCHOLQR = ( bs == 1 ) ? 0 : m * bs + 2 * m * bs * bs + fp_SCHOLQR + bs * bs + 2 * bs * bs * bs + fp_SKETCH;
    switch ( rgs_data->lss_alg ) {
        case RGS_HH:
            fp_LSS = k * nb * sumi + k * nb + nb * bs * sumi + 2 * k * nb * nb * sumi2 - ( 2 / 3 ) * nb * nb * nb * sumi3 + k * nb * bs * sumi + k * k * nb * sumi;
            break;
        case RGS_CGS:
        case RGS_BMGS:
        default:
            fp_LSS = 0;
            break;
    }

    rgs_data->rgs_flops = round( coef * ( fp_SKETCH + fp_L2SCHOLQR ) + fp_LSS + fp_Qupd );
}

void
base_memory_rgs( base_mem_t *rgs_memory, base_mem_t sketch_memory, size_t mem_in_bytes )
{
    *rgs_memory = base_mem_add( sketch_memory, set_mem_in_units( mem_in_bytes ) );
}