#include "gdsm.h"

void
dinitorthobasis_sincos( double *mat, base_int_t n, base_int_t m, base_matrix_layout_e layout )
{
    assert( mat != NULL );
    assert( n > 1 );
    assert( m > 1 );
    double dx  = base_d_p1 / ( n - 1 );
    double dmu = base_d_p1 / ( m - 1 );

    switch ( layout ) {
        case BASE_ROWMAJOR:
            for ( base_int_t i = 0; i < n; ++i ) {
                base_int_t k = i * m;
                double     x = i * dx;
                for ( base_int_t j = 0; j < m; ++j ) {
                    mat[j + k] = dorthobasis_sincos( x, j * dmu );
                }
            }
            break;
        case BASE_COLMAJOR:
        default:
            for ( base_int_t j = 0; j < m; j++ ) {
                base_int_t k  = j * n;
                double     mu = j * dmu;
                for ( base_int_t i = 0; i < n; i++ ) {
                    mat[i + k] = dorthobasis_sincos( i * dx, mu );
                }
            }
            break;
    }
}

void
dinitlinear( double *mat, base_int_t nr, base_int_t nc, base_matrix_layout_e layout )
{
    base_int_t i = 0, k = 0, tmp = 0;
    if ( layout == BASE_ROWMAJOR ) {
        tmp = nc;
        nc  = nr;
        nr  = tmp;
    }
    for ( ; i < nc; ++i ) {
        k = i * nr;
        for ( base_int_t j = 0; j < nr; ++j ) {
            mat[j + k] = (double)( j + k );
        }
    }
}

void
dinitwrandrange( double *mat, base_int_t nele, double min, double max, base_uint_t se )
{
    if ( se > 0 ) {
        srand( se );
    }
    else if ( !se ) {  // i.e se==0
        srand( (base_uint_t)time( NULL ) );
    }
    base_int_t i     = 0;
    double     scale = ( max - min ) / (double)RAND_MAX;
    for ( ; i < nele; ++i ) {
        mat[i] = min + scale * (double)rand();
    }
}

void
dinitlarnv( double *mat, int rank, base_int_t localsize, base_int_t ncols, base_int_t nb, int cite )
{
    int        ierr      = 0;
    base_int_t remainder = ncols % nb;
    base_int_t coef      = ( ncols - remainder ) / nb;
    if ( remainder > 0 )
        coef += 1;
    base_uint_t Rseed = 1234567;
    base_int_t  cur   = ( remainder == 0 ) ? nb : ( ( cite < coef - 1 ) ? nb : remainder );
    srand( rank * Rseed + cite * Rseed );
    int iseed[4] = { rand() % 4095, rand() % 4095, rand() % 4095, 5 };
    ierr         = LAPACKE_dlarnv( 3, iseed, localsize * cur, mat );
    if ( ierr != 0 ) {
        printf( "ierr = %d\n", ierr );
        exit( 1 );
    }
}

void
dinitmmiodense( double **mat, size_t *mat_capacity, base_int_t *m, base_int_t *n, base_int_t *nz, base_int_t *lda, char *fmm, base_matrix_layout_e layout )
{
    int         ret_code;
    MM_typecode matcode;
    FILE       *f;
    size_t      i;
    size_t      min_capacity = 0;
    printf( "%s\n", fmm );
    if ( ( f = fopen( fmm, "r" ) ) == NULL ) {
        PRINT_INFO();
        exit( 1 );
    }
    if ( mm_read_banner( f, &matcode ) != 0 ) {
        printf( "Could not process Matrix Market banner.\n" );
        exit( 1 );
    }

    char *type_str = mm_typecode_to_str( matcode );
    if ( mm_is_integer( matcode ) || mm_is_pattern( matcode ) || mm_is_complex( matcode ) || !mm_is_matrix( matcode ) ) {
        printf( "Sorry, this application does not support " );
        printf( "Market Market type: [%s]\n", type_str );
        exit( 1 );
    }
    printf( "[%s]\n", type_str );
    free( type_str );

    switch ( matcode[1] ) {
        case 'C':  // mm_is_sparse || mm_is_coordinate
            if ( ( ret_code = mm_read_mtx_crd_size( f, m, n, nz ) ) != 0 ) {
                exit( 0 );
            }

            min_capacity = (size_t)( *m ) * (size_t)( *n );
            if ( ( ( mat_capacity == NULL ) && ( *mat == NULL ) ) ||
                 ( ( mat_capacity != NULL ) && ( *mat == NULL ) ) ) {  // on veut allouer un pointeur initialiser et retourner le pointeur vers le bloc alloue
                *mat_capacity = min_capacity;
                *lda          = ( layout == BASE_COLMAJOR ) ? ( *m ) : ( *m );
                CHECKSPEALLOC( *mat, base_dalign, (size_t)( *m ) * (size_t)( *n ) * sizeof( double ) );
            }
            else if ( *mat_capacity < min_capacity ) {
                PRINT_MSGERR( "mat_capacity < min_capacity" );
                exit( 0 );
            }
            else if ( *lda < ( ( layout == BASE_COLMAJOR ) ? ( *m ) : ( *m ) ) ) {
                PRINT_MSGERR( "lda is less than m if mat is COL MAJOR or less then n if mat is ROW MAJOR" );
                exit( 0 );
            }
            else if ( *mat != NULL && *mat_capacity >= min_capacity ) {
            }

            memset( *mat, 0, (size_t)( *m ) * (size_t)( *n ) * sizeof( double ) );
            base_int_t *row_idx = NULL, *col_idx = NULL;
            double     *val = NULL;
            CHECKSPEALLOC( row_idx, base_ialign, (size_t)( *nz ) * sizeof( base_int_t ) );
            CHECKSPEALLOC( col_idx, base_ialign, (size_t)( *nz ) * sizeof( base_int_t ) );
            CHECKSPEALLOC( val, base_dalign, (size_t)( *nz ) * sizeof( double ) );
            /* NOTE: when reading in doubles, ANSI C requires the use of the "l"  */
            /*   specifier as in "%lg", "%lf", "%le", otherwise errors will occur */
            /*  (ANSI C X3.159-1989, Sec. 4.9.6.2, p. 136 lines 13-15)            */
            printf( "m %" BASE_PRId " n %" BASE_PRId " nz %" BASE_PRId "\n", *m, *n, *nz );
            for ( i = 0; i < (size_t)( *nz ); i++ ) {
                fscanf( f, "%" BASE_SCNd " %" BASE_SCNd " %lg", &row_idx[i], &col_idx[i], &val[i] );
                row_idx[i]--; /* adjust from 1-based to 0-based */
                col_idx[i]--;
                // size_t k    = ( layout == BASE_COLMAJOR ) ? ( row_idx[i] + col_idx[i] * (size_t)( *m ) ) : ( row_idx[i] * (size_t)( *n ) + col_idx[i] );
                size_t k    = ( layout == BASE_COLMAJOR ) ? ( row_idx[i] + col_idx[i] * (size_t)( *lda ) ) : ( row_idx[i] * (size_t)( *lda ) + col_idx[i] );
                ( *mat )[k] = val[i];  // sparse to dense filling
                if ( mm_is_symmetric( matcode ) && row_idx[i] != col_idx[i] ) {
                    // size_t k_sym    = ( layout == BASE_COLMAJOR ) ? ( col_idx[i] + row_idx[i] * (size_t)( *m ) ) : ( col_idx[i] * (size_t)( *n ) + row_idx[i] );
                    size_t k_sym    = ( layout == BASE_COLMAJOR ) ? ( col_idx[i] + row_idx[i] * (size_t)( *lda ) ) : ( col_idx[i] * (size_t)( *lda ) + row_idx[i] );
                    ( *mat )[k_sym] = val[i];
                }
            }
            SPEFREE( row_idx );
            SPEFREE( col_idx );
            SPEFREE( val );
            break;
        case 'A':  // mm_is_array || mm_is_dense
        default:
            *nz = -1;
            if ( ( ret_code = mm_read_mtx_array_size( f, m, n ) ) != 0 ) {
                exit( 0 );
            }

            min_capacity = (size_t)( *m ) * (size_t)( *n );
            if ( ( ( mat_capacity == NULL ) && ( *mat == NULL ) ) ||
                 ( ( mat_capacity != NULL ) && ( *mat == NULL ) ) ) {  // on veut allouer un pointeur initialiser et retourner le pointeur vers le bloc alloue
                *mat_capacity = min_capacity;
                *lda          = ( layout == BASE_COLMAJOR ) ? ( *m ) : ( *m );
                CHECKSPEALLOC( *mat, base_dalign, (size_t)( *m ) * (size_t)( *n ) * sizeof( double ) );
            }
            else if ( *mat_capacity < min_capacity ) {
                PRINT_MSGERR( "mat_capacity < min_capacity" );
                exit( 0 );
            }
            else if ( *lda < ( ( layout == BASE_COLMAJOR ) ? ( *m ) : ( *m ) ) ) {
                PRINT_MSGERR( "lda is less than m if mat is COL MAJOR or less then n if mat is ROW MAJOR" );
                exit( 0 );
            }
            else if ( *mat != NULL && *mat_capacity >= min_capacity ) {
            }

            double v = 0;
            for ( size_t j = 0; j < (size_t)( *n ); ++j ) {
                for ( i = 0; i < (size_t)( *m ); ++i ) {
                    // size_t k = ( layout == BASE_COLMAJOR ) ? ( i + j * (size_t)( *m ) ) : ( i * (size_t)( *n ) + j );
                    size_t k = ( layout == BASE_COLMAJOR ) ? ( i + j * (size_t)( *lda ) ) : ( i * (size_t)( *lda ) + j );
                    fscanf( f, "%lg", &v );
                    ( *mat )[k] = v;
                }
            }
            break;
    }
    if ( f != stdin ) {
        fclose( f );
    }
}