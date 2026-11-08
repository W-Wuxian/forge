#ifndef __BASE_DATATYPE_H__
#define __BASE_DATATYPE_H__

#if defined( BASE_INT64 )

typedef int64_t  base_int_t;
typedef uint64_t base_uint_t;
#define BASE_INT_MAX INT64_MAX
#define BASE_INT_MIN INT64_MIN

#elif defined( BASE_INT32 )

typedef int32_t  base_int_t;
typedef uint32_t base_uint_t;
#define BASE_INT_MAX INT32_MAX
#define BASE_INT_MIN INT32_MIN

#elif defined( BASE_LONG )

typedef long          base_int_t;
typedef unsigned long base_uint_t;
#define BASE_INT_MAX LONG_MAX
#define BASE_INT_MIN LONG_MIN

#elif defined( BASE_INT )

typedef int          base_int_t;
typedef unsigned int base_uint_t;
#define BASE_INT_MAX INT_MAX
#define BASE_INT_MIN INT_MIN

#else

typedef int64_t  base_int_t;
typedef uint64_t base_uint_t;
#define BASE_INT_MAX INT64_MAX
#define BASE_INT_MIN INT64_MIN

#endif

#define base_ialign 64
#define base_dalign 64

#define fix_dbl double
#define fix_flt float

// p1 -> +1  m1 -> -1   ze -> 0.0
#define base_d_p1 (fix_dbl)1.0
#define base_d_m1 ( fix_dbl ) - 1.0
#define base_d_ze (fix_dbl)0.0
#define base_d_p2 (fix_dbl)2.0
#define base_d_p3 (fix_dbl)3.0
#define base_d_p6 (fix_dbl)6.0

#define base_d_beta_ze base_d_ze
#define base_d_beta_p1 base_d_p1
#define base_d_alpha_p1 base_d_p1
#define base_d_alpha_m1 base_d_m1

typedef enum { BASE_ROWMAJOR, BASE_COLMAJOR } base_matrix_layout_e;

static inline void
_dinitlinear( double *mat, base_int_t nr, base_int_t nc, base_matrix_layout_e layout )
{
    // base_int_t i = 0, k = 0;
    // if ( layout == BASE_ROWMAJOR ) {
    //     for ( ; i < nr; ++i ) {
    //         k = i * nc;
    //         for ( base_int_t j = 0; j < nc; ++j ) {
    //             mat[j + k] = (double)( j + k );
    //         }
    //     }
    // }
    // else if ( layout == BASE_COLMAJOR ) {
    //     for ( ; i < nc; ++i ) {
    //         k = i * nr;
    //         for ( base_int_t j = 0; j < nr; ++j ) {
    //             mat[j + k] = (double)( j + k );
    //         }
    //     }
    // }

    base_int_t i = 0, k = 0, tmp = 0;
    if ( layout == BASE_COLMAJOR ) {
        tmp = nr;
        nr  = nc;
        nc  = tmp;
    }
    for ( ; i < nr; ++i ) {
        k = i * nc;
        for ( base_int_t j = 0; j < nc; ++j ) {
            mat[j + k] = (double)( j + k );
        }
    }
}

static inline void
_dinitwrandrange( double *mat, base_int_t nele, double min, double max, base_uint_t se )
{
    if ( se ) {
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

static inline base_int_t
base_imin( base_int_t a, base_int_t b )
{
    return ( a < b ) ? a : b;
}

static inline base_int_t
base_imax( base_int_t a, base_int_t b )
{
    return ( a > b ) ? a : b;
}

typedef struct {
    fix_dbl mem_in_kb;
    fix_dbl mem_in_mb;
    fix_dbl mem_in_gb;
} base_mem_s;

typedef base_mem_s base_mem_t;

#define BASE_KB ( 1024 )
#define BASE_MB ( 1024 * 1024 )
#define BASE_GB ( 1024 * 1024 * 1024 )

// static inline fix_dbl
// set_mem_in_kb( size_t a )
// {
//     return (fix_dbl)ceil( (fix_dbl)a / BASE_KB );
// }
// static inline fix_dbl
// set_mem_in_mb( size_t a )
// {
//     return (fix_dbl)ceil( (fix_dbl)a / BASE_MB );
// }
// static inline fix_dbl
// set_mem_in_gb( size_t a )
// {
//     return (fix_dbl)ceil( (fix_dbl)a / BASE_GB );
// }

static inline fix_dbl
set_mem_in_kb( size_t a )
{
    return (fix_dbl)( (fix_dbl)a / BASE_KB );
}
static inline fix_dbl
set_mem_in_mb( size_t a )
{
    return (fix_dbl)( (fix_dbl)a / BASE_MB );
}
static inline fix_dbl
set_mem_in_gb( size_t a )
{
    return (fix_dbl)( (fix_dbl)a / BASE_GB );
}

static inline base_mem_t
set_mem_in_units( size_t a )
{
    return ( base_mem_t ){ .mem_in_kb = set_mem_in_kb( a ), .mem_in_mb = set_mem_in_mb( a ), .mem_in_gb = set_mem_in_gb( a ) };
}
static inline base_mem_t
base_mem_add( base_mem_t a, base_mem_t b )
{
    return ( base_mem_t ){ .mem_in_kb = ( a.mem_in_kb + b.mem_in_kb ), .mem_in_mb = ( a.mem_in_mb + b.mem_in_mb ), .mem_in_gb = ( a.mem_in_gb + b.mem_in_gb ) };
}

static inline void
base_display_stats( const base_mem_t a, const fix_dbl flops, const char *msg )
{
    printf( "%s\n", msg );
    printf( "\r%g Flops:\n", flops );
    printf( "\r%g KB\n", a.mem_in_kb );
    printf( "\r%g MB\n", a.mem_in_mb );
    printf( "\r%g GB\n", a.mem_in_gb );
}

#endif  //__BASE_DATATYPE_H__
