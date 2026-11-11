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
    MPI_Comm   comm       = NULL;
    int        ierr       = 0;
    int        rank       = 0;
    int        size       = 1;
    int        thread_nb  = 1;
    base_int_t globalsize = NROWS;
    base_int_t m = NROWS, n = NCOLS; /**< local nbr rows, columns */
    base_int_t sample = 10;          /** number of runs */
    double     nrm    = 0;
    ierr              = MPI_Comm_rank( comm, &rank );
    ierr              = MPI_Comm_size( comm, &size );

    distload( rank, size, globalsize, &m ); /**< Computes localsize */
    printf( "rank %d"
            " M %" BASE_PRId " m %" BASE_PRId " n %" BASE_PRId "\n",
            rank,
            globalsize,
            m,
            n );
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
    SPEALLOC( A, base_dalign, size_A );
    SPEALLOC( Q, base_dalign, size_Q );
    SPEALLOC( R, base_dalign, size_R );
    SPEALLOC( work, base_dalign, size_work );
    memset( Q, 0, size_Q );
    memset( R, 0, size_R );
    memset( work, 0, size_work );
    BASE_MATCREATE( A, dinitlarnv, rank, m, n, n, 0 ); /**< Generate a distributed block row-wise ColMajor matrix */
    PRINT_COLMAJ_MAT( A, m, n, "A:" );
    ierr = TSQR( comm, m, n, A, Q, R, work );
    PRINT_COLMAJ_MAT( Q, m, n, "Q:" );
    PRINT_COLMAJ_MAT( R, n, n, "R" );
    BASE_MATCREATE( A, dinitlarnv, rank, m, n, n, 0 ); /**< Generate a distributed block row-wise ColMajor matrix */
    nrm = ( size > 1 ) ? base_distbackward_error( A, m, n, Q, R, n, comm ) : base_backward_error( A, m, n, Q, R, n );
    printf( "nrm = %lE\n", nrm );
    SPEFREE( A );
    SPEFREE( Q );
    SPEFREE( R );
    SPEFREE( work );
    MPI_Barrier( comm );
    MPI_Finalize();
    return ierr;
}