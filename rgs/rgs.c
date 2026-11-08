#include "rgs.h"

void
base_set_rgs_data( base_rgs_t *rgs_data, base_int_t *iparam, int rank, int size, MPI_Comm comm )
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
    rgs_data->current_block_size                    = ( iparam[IDX_USER_BLOCK_SIZE] > rgs_data->remaining_block_size ) ? iparam[IDX_USER_BLOCK_SIZE] : rgs_data->remaining_block_size;
    base_int_t sketch_iparam[IDX_SKETCH_IPARAM_LEN] = { iparam[IDX_NROWS_DATA_IN], rgs_data->current_block_size, iparam[IDX_SKETCH_DIM],         iparam[IDX_NDR],           iparam[IDX_NDL],
                                                        iparam[IDX_SKETCH_ALG],    iparam[IDX_SKETCH_TYPE],      iparam[IDX_SKETCH_NUM_THREADS], iparam[IDX_SKETCH_NUM_GPU] };
    // Init SKETCH
    base_init_sketch_data( &rgs_data->sketch_data, sketch_iparam, rank, size );
    // Set mem
    base_int_t size_tmp_data_work = ( rgs_data->sketch_dim * rgs_data->current_block_size );
    base_int_t Stmp               = ( rgs_data->sketch_dim * rgs_data->current_block_size * rgs_data->number_of_block );
    base_int_t worktmp            = ( 2 * rgs_data->ncols_data_in * rgs_data->ncols_data_in ) * ( size + 1 );
    base_int_t addtmp             = base_imax( Stmp, worktmp );
    rgs_data->len_tmp_data_work   = addtmp;
    size_tmp_data_work += addtmp;
    rgs_data->size_tmp_data_work  = size_tmp_data_work;
    base_int_t size_pske          = rgs_data->sketch_dim * rgs_data->ncols_data_in;
    base_int_t size_sske          = rgs_data->sketch_dim * rgs_data->ncols_data_in;
    base_int_t size_rfac          = rgs_data->ncols_data_in * rgs_data->ncols_data_in;
    base_int_t size_qfac          = rgs_data->nrows_data_in * rgs_data->ncols_data_in;
    base_int_t size_tmp_data_tsqr = rgs_data->nrows_data_in * rgs_data->user_block_size;

    base_int_t size_data_work = size_tmp_data_work + size_pske + size_sske + size_tmp_data_tsqr;
    size_data_work += ( size_rfac + size_qfac );
    rgs_data->size_data_work = size_data_work;
    SPEALLOC( rgs_data->data_work, base_dalign, size_data_work * sizeof( double ) );
    BASE_ASSERT_ISNOTNULL( rgs_data->data_work );
    // Set memory to Zero
    memset( rgs_data->data_work, 0, size_data_work * sizeof( double ) );
    // Set ptr
    base_int_t offset       = 0;
    rgs_data->tmp_data_work = rgs_data->data_work;
    offset += size_tmp_data_work;
    rgs_data->pske = rgs_data->data_work + offset;
    offset += size_pske;
    rgs_data->sske = rgs_data->data_work + offset;
    offset += size_sske;
    rgs_data->rfac = rgs_data->data_work + offset;
    offset += size_rfac;
    rgs_data->qfac = rgs_data->data_work + offset;
    offset += size_qfac;
    rgs_data->tmp_data_tsqr = rgs_data->data_work + offset;
    offset += size_tmp_data_tsqr;
    // Set Sketch
    base_set_sketch_data( &rgs_data->sketch_data, rank, size );
    BASE_ASSERT_ISNOTNULL( ( rgs_data->sketch_data.data_work ) );

    // Approx memory used and flops
    base_int_t sum = ( size_pske + size_sske + size_qfac + size_tmp_data_tsqr + size_rfac + size_tmp_data_work );
    base_memory_rgs( &rgs_data->rgs_memory, rgs_data->sketch_data.sketch_memory, (size_t)( sum * sizeof( double ) ) );
    base_flops_rgs( rgs_data );
}

void
base_get_data_rgs( base_rgs_t *rgs_data, int rgs_ite, double *data_need_facto )
{
    int ierr                  = 0;
    rgs_data->current_rgs_ite = rgs_ite;
    rgs_data->current_block_size =
        ( rgs_data->remaining_block_size == 0 ) ? rgs_data->user_block_size : ( ( rgs_ite < ( rgs_data->number_of_block ) - 1 ) ? rgs_data->user_block_size : rgs_data->remaining_block_size );
    rgs_data->data_need_facto = data_need_facto;
    BASE_ASSERT_ISNOTNULL( rgs_data->data_work );
    BASE_ASSERT_ISNOTNULL( rgs_data->qfac );
    ierr = LAPACKE_dlacpy( LAPACK_COL_MAJOR,
                           'A',
                           rgs_data->nrows_data_in,
                           rgs_data->current_block_size,
                           data_need_facto,
                           rgs_data->nrows_data_in,
                           rgs_data->qfac + rgs_ite * rgs_data->user_block_size * rgs_data->nrows_data_in,
                           rgs_data->nrows_data_in );
    if ( ierr != 0 ) {
        if ( rgs_data->rank == 0 )
            PRINT_MSGERR( "LAPACKE_dlacpy ierr= ", ierr );
        exit( 1 );
    }
}

void
base_compute_rgs( base_rgs_t *rgs_data )
{
    // tmp data
    /* base_int_t    tmp_offset = 0;
    double *const rtmp       = rgs_data->tmp_data_work;
    tmp_offset += rgs_data->sketch_dim * rgs_data->current_block_size;
    double *const stmp = rgs_data->tmp_data_work + tmp_offset; */
    // current location to update
    base_int_t rite_offset   = rgs_data->current_rgs_ite * rgs_data->ncols_data_in * rgs_data->user_block_size;
    base_int_t skeite_offset = rgs_data->current_rgs_ite * rgs_data->sketch_dim * rgs_data->user_block_size;
    base_int_t qite_offset   = rgs_data->current_rgs_ite * rgs_data->nrows_data_in * rgs_data->user_block_size;
    double    *rite          = rgs_data->rfac + rite_offset;
    double    *pskeite       = rgs_data->pske + skeite_offset;
    double    *sskeite       = rgs_data->sske + skeite_offset;
    double    *qite          = rgs_data->qfac + qite_offset;
    // norm
    double invnrm = base_d_ze;
    /**< Sketch the input object (W==data_need_facto) W_{ite} as (PSkeite=)P_{ite} := Theta W_{ite} */
    base_getdata_sketch_data( &rgs_data->sketch_data, rgs_data->current_block_size, rgs_data->data_need_facto, pskeite );
    base_compute_block_sketch_nocomm( &rgs_data->sketch_data );
    if ( rgs_data->current_rgs_ite > 0 ) {
        /**< Solve the Least-Squares problem ||S_{:,0:ite-1}X -  P_{ite}|| with (R_{0:ite-1, ite} = X) */
        _base_lss_rgs( rgs_data );
        /**< Update the Q factor as Q_{:,ite} = W_{:,ite} - Q_{:,0:ite-1}R_{0:ite-1, ite} */
        cblas_dgemm( CblasColMajor,
                     CblasNoTrans,
                     CblasNoTrans,
                     rgs_data->nrows_data_in,
                     rgs_data->current_block_size,
                     rgs_data->current_rgs_ite * rgs_data->user_block_size,
                     base_d_alpha_m1,
                     rgs_data->qfac,
                     rgs_data->nrows_data_in,
                     rite,
                     rgs_data->ncols_data_in,
                     base_d_beta_p1,
                     qite,
                     rgs_data->nrows_data_in );
    }
    if ( rgs_data->current_block_size > 1 ) { /**< Randomized Block Gram-Schmidt :: Computes R_{ite,ite} & S_{:,ite}*/
        if ( rgs_data->current_rgs_ite == 0 ) {
            base_l2qr_sketched_cholqr( rgs_data );
        }
        else {
            for ( base_int_t i = 0; i < rgs_data->number_reortho_sketched_cholqr; ++i ) {
                base_sketched_cholqr( rgs_data );
                // Compute S = S'R''^{-1} with trsm or BSKETCH
                base_getdata_sketch_data( &rgs_data->sketch_data, rgs_data->current_block_size, qite, sskeite );
                base_compute_block_sketch_nocomm( &rgs_data->sketch_data );
            }
        }
    }
    else { /**< Randomized Gram-Schmidt */
        base_getdata_sketch_data( &rgs_data->sketch_data, rgs_data->current_block_size, qite, sskeite );
        base_compute_block_sketch_nocomm( &rgs_data->sketch_data );
        /**< Compute norm(si) */
        rite[rgs_data->current_rgs_ite] = cblas_dnrm2( rgs_data->sketch_dim, sskeite, 1 ); /**< Update R[ite,ite] */
        invnrm                          = base_d_p1 / rite[rgs_data->current_rgs_ite];
        /**< Scale si, qi */
        cblas_dscal( rgs_data->sketch_dim, invnrm, sskeite, 1 );
        cblas_dscal( rgs_data->nrows_data_in, invnrm, qite, 1 );
    }
}

void
_base_lss_rgs( base_rgs_t *rgs_data )
{
    int ierr = 0;
    int j    = 0;
    // Renaming
    int        l             = rgs_data->number_reortho_lss;
    base_int_t Kproj         = rgs_data->sketch_dim;
    base_int_t nblocks       = rgs_data->user_block_size;
    base_int_t blocks_size   = rgs_data->current_block_size;
    base_int_t ncols         = rgs_data->ncols_data_in;
    int        ite           = rgs_data->current_block_size;
    double    *Rite          = rgs_data->rfac + ite * ncols * nblocks;
    base_int_t Skeite_offset = ite * Kproj * nblocks;
    double    *PSkeite       = rgs_data->pske + Skeite_offset;
    //  tmp data
    base_int_t tmp_offset = 0;
    double    *Rtmp       = rgs_data->tmp_data_work + tmp_offset;
    tmp_offset += Kproj * blocks_size;
    double *Stmp = rgs_data->tmp_data_work + tmp_offset;
    /**< Least-Squares Solver*/
    switch ( rgs_data->lss_alg ) {
        case RGS_BCGLS: /** TODO Block Conjugate Gradient Least Squares (BCGLS) solving S^TS X = S^TP */
            break;
        case RGS_HH: /**< ?gels uses QR-factorization (?geqrf) to solve overdetermined system: ||S_{:,0:ite-1}X - Rtmp||_{2} */
            ierr = LAPACKE_dlacpy( LAPACK_COL_MAJOR, 'A', Kproj, ite * nblocks, rgs_data->sske, Kproj, Stmp, Kproj );
            if ( ierr != 0 ) {
                if ( rgs_data->rank == 0 )
                    PRINT_MSGERR( "LAPACKE_dlacpy ierr= ", ierr );
                exit( 1 );
            }
            ierr = LAPACKE_dlacpy( LAPACK_COL_MAJOR, 'A', Kproj, blocks_size, PSkeite, Kproj, Rtmp, Kproj );
            if ( ierr != 0 ) {
                if ( rgs_data->rank == 0 )
                    PRINT_MSGERR( "LAPACKE_dlacpy ierr= ", ierr );
                exit( 1 );
            }
            ierr = LAPACKE_dgels( LAPACK_COL_MAJOR, 'N', Kproj, ite * nblocks, blocks_size, Stmp, Kproj, Rtmp, Kproj );
            if ( ierr != 0 ) {
                if ( rgs_data->rank == 0 )
                    PRINT_MSGERR( "LAPACKE_dgels ierr= ", ierr );
                exit( 1 );
            }
            /**< Update R_{0:ite-1, ite} with the solution returned by ?gels */
            ierr = LAPACKE_dlacpy( LAPACK_COL_MAJOR, 'A', ite * nblocks, blocks_size, Rtmp, Kproj, Rite, ncols );
            if ( ierr != 0 ) {
                if ( rgs_data->rank == 0 )
                    PRINT_MSGERR( "LAPACKE_dlacpy ierr= ", ierr );
                exit( 1 );
            }
            break;
        case RGS_BMGS: /**< Block Modified Gram-Schmidt process with l re-orthogonalization */
            while ( l ) {
                ierr = LAPACKE_dlacpy( LAPACK_COL_MAJOR, 'A', Kproj, blocks_size, PSkeite, Kproj, Rtmp, Kproj );
                if ( ierr != 0 ) {
                    if ( rgs_data->rank == 0 )
                        PRINT_MSGERR( "LAPACKE_dlacpy ierr= ", ierr );
                    exit( 1 );
                }
                for ( j = 0; j < rgs_data->current_rgs_ite; ++j ) {
                    /**< P_{ite} = P_{ite} - S_{j}R_{j, ite} */
                    cblas_dgemm( CblasColMajor,
                                 CblasNoTrans,
                                 CblasNoTrans,
                                 Kproj,
                                 blocks_size,
                                 nblocks,
                                 base_d_alpha_m1,
                                 rgs_data->sske + ( j * Kproj * nblocks ),
                                 Kproj,
                                 Rite + j * nblocks,
                                 ncols,
                                 base_d_beta_p1,
                                 Rtmp,
                                 Kproj );
                    /**< R_{j, ite} = R_{j, ite} - S^{T}_{j}P_{ite} */
                    cblas_dgemm( CblasColMajor,
                                 CblasTrans,
                                 CblasNoTrans,
                                 nblocks,
                                 blocks_size,
                                 Kproj,
                                 base_d_alpha_p1,
                                 rgs_data->sske + ( j * Kproj * nblocks ),
                                 Kproj,
                                 Rtmp,
                                 Kproj,
                                 base_d_beta_p1,
                                 Rite + j * nblocks,
                                 ncols );
                }
                --l;
            }
            break;
        case RGS_CGS: /**< Classical Gram-Schmidt with l re-orthogonalization (Richardson iterations) */
        default:
            cblas_dgemm(
                CblasColMajor, CblasTrans, CblasNoTrans, ite * nblocks, blocks_size, Kproj, base_d_alpha_m1, rgs_data->sske, Kproj, PSkeite, Kproj, base_d_beta_ze, Rite, ncols ); /**< Rite <- S^TP
                                                                                                                                                                                    */
            while ( l - 1 ) {
                ierr = LAPACKE_dlacpy( LAPACK_COL_MAJOR, 'A', Kproj, blocks_size, PSkeite, Kproj, Rtmp, Kproj );
                if ( ierr != 0 ) {
                    if ( rgs_data->rank == 0 )
                        PRINT_MSGERR( "LAPACKE_dlacpy ierr= ", ierr );
                    exit( 1 );
                }
                /**< Rtmp = Rtmp - S_{:,0:ite-1}R_{0:ite-1,ite} */
                cblas_dgemm( CblasColMajor, CblasNoTrans, CblasNoTrans, Kproj, blocks_size, ite * nblocks, base_d_alpha_m1, rgs_data->sske, Kproj, Rite, ncols, base_d_beta_p1, Rtmp, Kproj );
                /**< R_{0:ite-1,ite} = R_{0:ite-1,ite} - S^{T}_{:,0:ite-1}Rtmp */
                cblas_dgemm( CblasColMajor, CblasTrans, CblasNoTrans, ite * nblocks, blocks_size, Kproj, base_d_alpha_p1, rgs_data->sske, Kproj, Rtmp, Kproj, base_d_beta_p1, Rite, ncols );
                --l;
            }
            break;
    }
}

void
base_sketched_cholqr( base_rgs_t *rgs_data )
{
    int ierr = 0;
    // Renaming
    //  MPI data
    base_int_t j = 0;
    /* int        rank = rgs_data->rank;
    int        size = rgs_data->size;
    MPI_Comm   comm = rgs_data->comm; */
    //  Data dims
    base_int_t Kproj       = rgs_data->sketch_dim;
    base_int_t ncols       = rgs_data->ncols_data_in;
    base_int_t nIn         = rgs_data->nrows_data_in;
    base_int_t ite         = rgs_data->current_rgs_ite;
    base_int_t blocks_size = rgs_data->current_block_size;
    base_int_t nblocks     = rgs_data->user_block_size;
    //  current location to update
    base_int_t Skeite_offset  = ite * Kproj * nblocks;
    double    *SSkeite        = rgs_data->sske + Skeite_offset;
    base_int_t Qite_offset    = ite * nIn * nblocks;
    double    *Qite           = rgs_data->qfac + Qite_offset;
    base_int_t Riteite_offset = ( ite * ncols * nblocks ) + ( ite * blocks_size );  // R is ncols x ncols offset is past computation + computed R_{0:ite-1, ite} (like)
    double    *Riteite        = rgs_data->rfac + Riteite_offset;
    // Compute Si := Theta Qi (SSkeite)
    base_getdata_sketch_data( &rgs_data->sketch_data, blocks_size, Qite, SSkeite );
    base_compute_block_sketch_nocomm( &rgs_data->sketch_data );
    // Compute STS Store the results in Riteite
    cblas_dgemm( CblasColMajor, CblasTrans, CblasNoTrans, blocks_size, blocks_size, Kproj, base_d_alpha_p1, SSkeite, Kproj, SSkeite, Kproj, base_d_beta_ze, Riteite, ncols );
    // Compute R_{i,i} (is blocks_size x blocks_size) as the upper triangular cholesky factor of STS
    // S^{T}S = R^{T}Q^{T}QR = LL^T = U^{T}U with Upper triangular matrix U = R and
    // Lower triangular matrix L = R^T
    ierr = LAPACKE_dpotrf( LAPACK_COL_MAJOR, 'U', blocks_size, Riteite, ncols );  // the Upper triangular part of STS is rewrite with U(=R)
    if ( ierr != 0 ) {
        if ( rgs_data->rank == 0 )
            PRINT_MSGERR( "LAPACKE_dpotrf ierr= ", ierr );
        exit( 1 );
    }
    // Zeroed strictly lower part of R
    for ( j = 0; j < blocks_size - 1; ++j )
        memset( Riteite + ( 1 + j + j * ncols ), 0, sizeof( double ) * ( blocks_size - ( j + 1 ) ) );
    // Compute Q = Q^{*}R^{-1} with trsm
    cblas_dtrsm( CblasColMajor, CblasRight, CblasUpper, CblasNoTrans, CblasNonUnit, nIn, blocks_size, base_d_alpha_p1, Riteite, ncols, Qite, nIn );  // X*op(A) = alpha*B
}

void
base_l2qr_sketched_cholqr( base_rgs_t *rgs_data )
{
    int ierr = 0;
    // Renaming
    //  MPI data
    /* int      rank = rgs_data->rank;
    int      size = rgs_data->size; */
    MPI_Comm comm = rgs_data->comm;
    //  Data dims
    base_int_t Kproj       = rgs_data->sketch_dim;
    base_int_t ncols       = rgs_data->ncols_data_in;
    base_int_t nIn         = rgs_data->nrows_data_in;
    int        ite         = rgs_data->current_rgs_ite;
    base_int_t blocks_size = rgs_data->current_block_size;
    base_int_t nblocks     = rgs_data->user_block_size;
    //  current location to update
    base_int_t Skeite_offset  = ite * Kproj * nblocks;
    double    *SSkeite        = rgs_data->sske + Skeite_offset;
    base_int_t Qite_offset    = ite * nIn * nblocks;
    double    *Qite           = rgs_data->qfac + Qite_offset;
    base_int_t Riteite_offset = ( ite * ncols * nblocks ) + ( ite * blocks_size );  // R is ncols x ncols offset is past computation + computed R_{0:ite-1, ite} (like)
    double    *Riteite        = rgs_data->rfac + Riteite_offset;
    double    *work           = rgs_data->tmp_data_work + ( Kproj * nblocks );  // work is (2blocks_size^2)*(size+1)
    // Compute L2-QR factorization of Q^{*} (=Q'R')
    ierr = LAPACKE_dlacpy( LAPACK_COL_MAJOR, 'A', nIn, blocks_size, Qite, nIn, rgs_data->tmp_data_tsqr, nIn );
    if ( ierr != 0 ) {
        if ( rgs_data->rank == 0 )
            PRINT_MSGERR( "LAPACKE_dlacpy ierr= ", ierr );
        exit( 1 );
    }
    ierr = TSQR( comm, nIn, blocks_size, rgs_data->tmp_data_tsqr, Qite, rgs_data->tmp_data_work, work );
    // Compute Sketched CholQR fact Q' = QR''
    base_sketched_cholqr( rgs_data );
    // Compute R''R' Store the results in Riteite using trmmm B:=alpha*op(A)*B or dgemm
    /*cblas_dtrmm(CblasColMajor, CblasRight, CblasUpper, CblasNoTrans, CblasNonUnit, blocks_size, blocks_size, 1.0, rgs_data->tmp_data_work, Kproj, Riteite, ncols);*/
    ierr = LAPACKE_dlacpy( LAPACK_COL_MAJOR, 'A', blocks_size, blocks_size, Riteite, ncols, work, blocks_size );
    if ( ierr != 0 ) {
        if ( rgs_data->rank == 0 )
            PRINT_MSGERR( "LAPACKE_dlacpy ierr= ", ierr );
        exit( 1 );
    }
    cblas_dgemm(
        CblasColMajor, CblasNoTrans, CblasNoTrans, blocks_size, blocks_size, blocks_size, base_d_alpha_p1, work, blocks_size, rgs_data->tmp_data_work, blocks_size, base_d_beta_ze, Riteite, ncols );
    // Compute S = S'R''^{-1} with trsm or BSKETCH
    /*cblas_dtrsm(CblasColMajor, CblasRight, CblasUpper, CblasNoTrans, CblasNonUnit, Kproj, blocks_size, 1.0,
     Riteite, ncols, SSkeite, Kproj); // X*op(A) = alpha*B*/
    base_getdata_sketch_data( &rgs_data->sketch_data, blocks_size, Qite, SSkeite );
    base_compute_block_sketch_nocomm( &rgs_data->sketch_data );
}

int
TSQR( MPI_Comm comm, base_int_t m, base_int_t n, double *A, double *Q, double *R, double *work )
{
    int ierr = 0;
    int size = 1;
    int rank = 0;
    /* MPI_Comm_rank( comm, &rank );
    MPI_Comm_size( comm, &size );
    if ( m < size * n ) {
        printf( "Number of local lines for TSQR is smaller than number of columns * number of procs.\nThis case is not treated in this subroutine\n" );
        exit( 1 );
    } */

    double *Q_s    = NULL;
    double *Atau_s = NULL;
    double *Qtau_s = NULL;
    double *OQ_s   = NULL;
    //	Symbolic pointer to tau of QR factorization of local matrix
    Atau_s = work; /**< Atau_s is n x n  */
    //	Symbolic pointer to the place to assemble the R matrices by Allgather
    Q_s = work + n * n; /**< Q_s is n x (size*n) */
    //	Symbolic pointer to tau of QR foctorization of the assembled matrix by Allgather
    Qtau_s = Q_s + n * n * size; /**< Qtau_s is n x n */
    //	Symbolic pointer to the reordered assembled matrix
    OQ_s = Qtau_s + n * n; /**< OQ_s is (size*n) x n */
    /*
     * work is (2n^2)(size+1) with the following layout:
     * [ Atau_s | Q_s | Qtau_s | OQ_s ]
     * Warning, the 4-th argument of LAPACKE_?geqrt and the 7-th argument of LAPACKE_?gemqrt is refered as nb in intel MKL documentation,
     * the block size used for the storage (?geqrt) or the block size used in the blocked QR (?gemqrt), we took nb === n
     * the 6-th argument of LAPACKE_?gemqrt is refered as k the number of elementary reflectors whose product defines the matrix Q, we took k === n
     */

    //	QR factorization of local matrix
    ierr = LAPACKE_dgeqrt( LAPACK_COL_MAJOR, m, n, n, A, m, work, n );
    if ( ierr != 0 ) {
        if ( rank == 0 )
            PRINT_MSGERR( "LAPACKE_dgeqrt ierr= ", ierr );
        exit( 1 );
    }
    //	Set R to zeros
    memset( R, 0, n * n * sizeof( double ) );
    //	Copy the upper triangular of the factorized matrix
    ierr = LAPACKE_dlacpy( LAPACK_COL_MAJOR, 'U', n, n, A, m, R, n );
    if ( ierr != 0 ) {
        if ( rank == 0 )
            PRINT_MSGERR( "LAPACKE_dlacpy ierr= ", ierr );
        exit( 1 );
    }
    //	Assemble the matrix of concatenated R

    // ierr = MPI_Allgather(R, n * n, MPI_DOUBLE, Q_s, n * n, MPI_DOUBLE, comm);

    //	Reorder in COL MAJOR
    /*
     * Q_s is (n x (size*n)) and after ?geqrt is .EQV. to [R_{0}|R_{1}|...|R_{h}|...|R_{size-1}], for h in [|0,size-1|], R_{h} is the R factor local to the process h
     * OQ_s is ((size*n) x n) and after the following loop, 0Q_s is .EQV. to (with s=size-1):
     *  -----
     * |R_{0}|
     * |R_{1}|
     * |  :  |
     * |R_{h}|
     * |  :  |
     * |R_{s}|
     *  -----
     */
    for ( base_int_t i = 0; i < n; i++ ) {
        for ( base_int_t j = 0; j < size; j++ ) {
#ifdef DEBUG
            assert( ( ( i * n * size + j * n ) < ( n * n * size ) ) ) assert( ( ( i * n + j * n * n ) < ( n * n * size ) ) )
#endif
                memcpy( OQ_s + i * n * size + j * n, Q_s + i * n + j * n * n, n * sizeof( double ) );
        }
    }
    //	QR factorization of the assembled matrix
    ierr = LAPACKE_dgeqrt( LAPACK_COL_MAJOR, n * size, n, n, OQ_s, n * size, Qtau_s, n );
    if ( ierr != 0 ) {
        if ( rank == 0 )
            PRINT_MSGERR( "LAPACKE_dgeqrt ierr= ", ierr );
        exit( 1 );
    }
    //	Copy the result R into the output parameter R
    ierr = LAPACKE_dlacpy( LAPACK_COL_MAJOR, 'U', n, n, OQ_s, n * size, R, n );
    if ( ierr != 0 ) {
        if ( rank == 0 )
            PRINT_MSGERR( "LAPACKE_dlacpy ierr= ", ierr );
        exit( 1 );
    }
    //	Set Q to 0
    memset( Q, 0, m * n * sizeof( double ) );
    //	Identity matrix in the first block of Q
    for ( unsigned int i = 0; i < n; i++ ) {
#ifdef DEBUG
        assert( ( ( i + m * i ) < ( m * n ) ) )
#endif
            Q[i + m * i] = 1.;
    }
    //	Apply the Q factor of the assembled matrix on Q
    ierr = LAPACKE_dgemqrt( LAPACK_COL_MAJOR, 'L', 'N', n * size, n, n, n, OQ_s, n * size, Qtau_s, n, Q, m );
    if ( ierr != 0 ) {
        if ( rank == 0 )
            PRINT_MSGERR( "LAPACKE_dgemqrt ierr= ", ierr );
        exit( 1 );
    }
    //	Put the block corresponding to my rank on the top of the result matrix
    ierr = LAPACKE_dlacpy( LAPACK_COL_MAJOR, 'A', n, n, Q + rank * n, m, Q, m );
    if ( ierr != 0 ) {
        if ( rank == 0 )
            PRINT_MSGERR( "LAPACKE_dlacpy ierr= ", ierr );
        exit( 1 );
    }
    //	Set under first block to 0
    for ( base_int_t i = 0; i < n; i++ ) {
#ifdef DEBUG
        assert( ( ( n + i * m ) < ( m * n ) ) )
#endif
            memset( Q + n + i * m, 0, ( m - n ) * sizeof( double ) );
    }
    //	Apply the Q factor of the local matrix on Q to have the local factor Q of the matrix A
    ierr = LAPACKE_dgemqrt( LAPACK_COL_MAJOR, 'L', 'N', m, n, n, n, A, m, Atau_s, n, Q, m );
    if ( ierr != 0 ) {
        if ( rank == 0 )
            PRINT_MSGERR( "LAPACKE_dgemqrt ierr= ", ierr );
        exit( 1 );
    }
    return ierr;
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
    double bs   = rgs_data->current_block_size;
    double nb   = rgs_data->user_block_size;
    double k    = rgs_data->sketch_dim;

    double fp_LSS       = 0;
    double sumi         = ( coef + base_d_p1 ) * ( coef / base_d_p2 );
    double sumi2        = ( coef * coef * coef / base_d_p3 ) - ( coef * coef / base_d_p2 ) + ( coef / base_d_p6 );
    double sumi3        = ( coef / base_d_p2 ) * ( coef + base_d_p1 );
    double fp_Qupd      = 2 * m * bs * ( 1 + nb * sumi );
    double fp_SKETCH    = (double)rgs_data->sketch_data.sketch_flops;
    double fp_SCHOLQR   = ( bs == 1 ) ? 0 : fp_SKETCH + 2 * bs * bs * k + ( bs * bs * bs / base_d_p3 ) + ( bs * bs / base_d_p2 ) + m * m * bs;
    double fp_L2SCHOLQR = ( bs == 1 ) ? 0 : m * bs + 2 * m * bs * bs + fp_SCHOLQR + bs * bs + 2 * bs * bs * bs + fp_SKETCH;
    switch ( rgs_data->lss_alg ) {
        case RGS_HH:
            fp_LSS = k * nb * sumi + k * nb + nb * bs * sumi + 2 * k * nb * nb * sumi2 - ( 2 / base_d_p3 ) * nb * nb * nb * sumi3 + k * nb * bs * sumi + k * k * nb * sumi;
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