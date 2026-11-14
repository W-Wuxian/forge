#include "rgs.h"

int
main( int argc, char **argv )
{
    MPI_Init( &argc, &argv );
    MPI_Comm   comm       = NULL;
    int        ierr       = 0;
    int        rank       = 0;
    int        size       = 1;
    int        thread_nb  = 1;
    base_int_t globalsize = 0;
    base_int_t m = 0, n = 0, nz = 0, lda = 0; /**< local nbr rows, columns */
    size_t     mat_capacity = 0;
    base_int_t sample       = 10; /** number of runs */
    double     nrm          = 0;
    ierr                    = MPI_Comm_rank( comm, &rank );
    ierr                    = MPI_Comm_size( comm, &size );

    double *A = NULL;
    // ../MMTX/e05r0000.mtx
    // ../MMTX/nos1.mtx
    BASE_MATCREATE( &A, dinitmmiodense, &mat_capacity, &m, &n, &nz, &lda, "../MMTX/nos1.mtx", BASE_COLMAJOR );
    PRINT_COLMAJ_MAT( A, base_imin( 10, m ), base_imin( 10, n ), "A:", m, base_i_ze, base_i_ze );
    globalsize = m;
    distload( rank, size, globalsize, &m ); /**< Computes localsize */
    printf( "rank %d"
            " M %" BASE_PRId " m %" BASE_PRId " n %" BASE_PRId "\n",
            rank,
            globalsize,
            m,
            n );
    // Alloc workspace
    const double alpha = base_d_alpha_p1, beta = base_d_beta_ze;

    double *Q         = NULL;
    double *R         = NULL;
    double *work      = NULL;
    double  size_Q    = m * n * sizeof( double );
    double  size_R    = n * n * sizeof( double );
    double  size_work = ( ( 2 * n * n ) * ( size + 1 ) ) * sizeof( double );

    SPEALLOC( Q, base_dalign, size_Q );
    SPEALLOC( R, base_dalign, size_R );
    SPEALLOC( work, base_dalign, size_work );
    memset( Q, 0, size_Q );
    memset( R, 0, size_R );
    memset( work, 0, size_work );

    ierr = TSQR( comm, m, n, A, Q, R, work );
    PRINT_COLMAJ_MAT( Q, base_imin( 10, m ), base_imin( 10, n ), "Q:", m, base_i_ze, base_i_ze );
    PRINT_COLMAJ_MAT( R, base_imin( 10, n ), base_imin( 10, n ), "R:", n, base_i_ze, base_i_ze );
    BASE_MATCREATE( &A, dinitmmiodense, &mat_capacity, &m, &n, &nz, &lda, "../MMTX/nos1.mtx", BASE_COLMAJOR );
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