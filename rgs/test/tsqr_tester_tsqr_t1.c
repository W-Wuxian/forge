#include "rgs.h"

#ifndef NROWS
#define NROWS 8
#endif

#ifndef NCOLS
#define NCOLS 2
#endif

int
main( int argc, char **argv )
{
    MPI_Init( &argc, &argv );
    int    ierr       = 0;
    int    rank       = 0;
    int    size       = 1;
    int    thread_nb  = 1;
    int    globalsize = NROWS;
    int    m = NROWS, n = NCOLS; /**< local nbr rows, columns */
    int    sample = 10;          /** number of runs */
    double nrm    = 0;
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );

    distload( rank, size, globalsize, &m ); /**< Computes localsize */
    printf( "rank %d M %d m %d n %d\n", rank, globalsize, m, n );
    // Alloc workspace
    const double alpha = base_d_alpha_p1, beta = base_d_beta_ze;
    double      *A         = NULL;
    double      *Q         = NULL;
    double      *R         = NULL;
    double      *work      = NULL;
    double       size_A    = m * n * sizeof( double );
    double       size_Q    = m * n * sizeof( double );
    double       size_R    = n * n * sizeof( double );
    double       size_work = ( ( 2 * n * n ) * ( size + 1 ) ) * sizeof( double );
    work                   = (double *)mkl_malloc( size_work, d_align );
    SPEALLOC( A, base_dalign, size_A );
    SPEALLOC( Q, base_dalign, size_Q );
    SPEALLOC( R, base_dalign, size_R );
    SPEALLOC( work, base_dalign, size_work );
    memset( Q, 0, size_Q );
    memset( R, 0, size_R );
    memset( work, 0, size_work );
    ierr = _preAlps_MATlarnv_d( A, rank, m, n, n, 0 ); /**< Generate a distributed block row-wise ColMajor matrix */
    CyclePrintMatD( A, m, n, m, "A", rank, size );
    ierr = TSQR( MPI_COMM_WORLD, m, n, A, Q, R, work, &TAG );
    CyclePrintMatD( Q, m, n, m, "Q", rank, size );
    CyclePrintMatD( R, n, n, n, "R", rank, size );
    ierr = _preAlps_MATlarnv_d( A, rank, m, n, n, 0 ); /**< Generate a distributed block row-wise ColMajor matrix */
    nrm  = ( size > 1 ) ? preAlps_DistMATBackward_Error_d( A, m, n, Q, R, n, MPI_COMM_WORLD ) : preAlps_MATBackward_Error_d( A, m, n, Q, R, n );
    printf( "nrm = %lE\n", nrm );
    mkl_free( A );
    mkl_free( Q );
    mkl_free( R );
    mkl_free( work );
    mkl_free_buffers();
    MPI_Barrier( MPI_COMM_WORLD );
    MPI_Finalize();
    return ierr;
}