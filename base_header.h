#ifndef __BASE_HEADER_H__
#define __BASE_HEADER_H__

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <cblas.h>
#include <lapacke.h>
#include <fftw3.h>
#include <fwht.h>

// #include "mkl.h"
#include "base_datatype.h"
// #include "mmio.h"

#ifdef _WIN32
#define SPEALLOC( ptr, align, size ) ( ( ( ptr ) = _aligned_malloc( ( size ), ( align ) ) ) == NULL ? -1 : 0 )
#elif defined( USE_POSIX_MEMALIGN )
#define SPEALLOC( ptr, align, size ) posix_memalign( (void **)&( ptr ), ( align ), ( size ) )
#elif defined( USE_MKL_MALLOC )
#define SPEALLOC( ptr, align, size ) ( ( ( ptr ) = mkl_malloc( ( size ), ( align ) ) ) == NULL ? -1 : 0 )
#elif defined( USE_MALLOC )
#define SPEALLOC( ptr, align, size ) ( ( ( ptr ) = malloc( size ) ) == NULL ? -1 : 0 )
#else
#define SPEALLOC( ptr, align, size ) ( ( ( ptr ) = malloc( size ) ) == NULL ? -1 : 0 )
#endif

#define CHECKSPEALLOC( ... )                                                                                                                                                                           \
    if ( SPEALLOC( __VA_ARGS__ ) != 0 ) {                                                                                                                                                              \
        PRINT_INFO();                                                                                                                                                                                  \
        exit( 1 );                                                                                                                                                                                     \
    }

#if defined( USE_MKL_MALLOC )
#define SPEFREE( ptr )                                                                                                                                                                                 \
    {                                                                                                                                                                                                  \
        mkl_free( ptr );                                                                                                                                                                               \
        ptr = NULL;                                                                                                                                                                                    \
    }
#else
#define SPEFREE( ptr )                                                                                                                                                                                 \
    {                                                                                                                                                                                                  \
        free( ptr );                                                                                                                                                                                   \
        ptr = NULL;                                                                                                                                                                                    \
    }
#endif

#define PRINT_INFO() printf( "In file:%s, function:%s() and line:%d\n", __FILE__, __FUNCTION__, __LINE__ )
// #define PRINT_MSGERR( a, ... ) printf( "%s %" BASE_PRId "\nIn file:%s, function:%s() and line:%d\n", ( a ), ##__VA_ARGS__, __FILE__, __FUNCTION__, __LINE__ )
//  #define PRINT_MSGIERR( a, ... ) printf( "%s %d \nIn file:%s, function:%s() and line:%d\n", ( a ), ##__VA_ARGS__, __FILE__, __FUNCTION__, __LINE__ )
//  1. Helper macros for 1 and 2 arguments
#define PRINT_MSGERR_1( a ) printf( "In %s from %s() at %d\n %s\n", __FILE__, __func__, __LINE__, ( a ) )
#define PRINT_MSGERR_2( a, b ) printf( "In %s from %s() at %d\n %s %" BASE_PRId "\n", __FILE__, __func__, __LINE__, ( a ), ( b ) )
// 2. Argument counter selector
#define GET_MSGERR_MACRO( _1, _2, NAME, ... ) NAME
// 3. Public macro
#define PRINT_MSGERR( ... ) GET_MSGERR_MACRO( __VA_ARGS__, PRINT_MSGERR_2, PRINT_MSGERR_1, UNUSED )( __VA_ARGS__ )
// 1. Helper macros for 1 and 2 arguments
#define PRINT_MSGIERR_1( a ) printf( "In %s from %s() at %d\n %s\n", __FILE__, __func__, __LINE__, ( a ) )
#define PRINT_MSGIERR_2( a, b ) printf( "In %s from %s() at %d\n %s %d\n", __FILE__, __func__, __LINE__, ( a ), ( b ) )
// 2. Argument counter selector
#define GET_MSGIERR_MACRO( _1, _2, NAME, ... ) NAME
// 3. Public macro
#define PRINT_MSGIERR( ... ) GET_MSGIERR_MACRO( __VA_ARGS__, PRINT_MSGIERR_2, PRINT_MSGIERR_1, UNUSED )( __VA_ARGS__ )

#if defined( NOASSERTNULL ) || defined( NDEBUG )
#define BASE_ASSERT_ISNOTNULL( a ) ( (void)0 )
#else
#define BASE_ASSERT_ISNOTNULL( a )                                                                                                                                                                     \
    do {                                                                                                                                                                                               \
        const void *const __a_ptr = ( a );                                                                                                                                                             \
        if ( __a_ptr == NULL ) {                                                                                                                                                                       \
            PRINT_INFO();                                                                                                                                                                              \
            printf( "assertion a != NULL failed:\n" );                                                                                                                                                 \
        }                                                                                                                                                                                              \
        assert( __a_ptr != NULL );                                                                                                                                                                     \
    } while ( 0 )
#endif

#if defined( NOASSERTINT ) || defined( NDEBUG )
#define BASE_ASSERT_INT( a, b ) ( (void)0 )
#define BASE_SILENT_ASSERT_INT( a, b ) ( (void)0 )
#define BASE_ASSERT_UINT( a, b ) ( (void)0 )
#define BASE_SILENT_ASSERT_UINT( a, b ) ( (void)0 )
#else
#define BASE_ASSERT_INT( a, b )                                                                                                                                                                        \
    do {                                                                                                                                                                                               \
        if ( ( a ) != ( b ) ) {                                                                                                                                                                        \
            PRINT_INFO();                                                                                                                                                                              \
            printf( "assertion a==b failed:\n" );                                                                                                                                                      \
            printf( "a %" BASE_PRId " b %" BASE_PRId "\n", (base_int_t)( a ), (base_int_t)( b ) );                                                                                                     \
        }                                                                                                                                                                                              \
        assert( ( a ) == ( b ) );                                                                                                                                                                      \
    } while ( 0 )

#define BASE_SILENT_ASSERT_INT( a, b )                                                                                                                                                                 \
    do {                                                                                                                                                                                               \
        if ( ( a ) != ( b ) ) {                                                                                                                                                                        \
            PRINT_INFO();                                                                                                                                                                              \
            printf( "assertion a==b failed:\n" );                                                                                                                                                      \
            printf( "a %" BASE_PRId " b %" BASE_PRId "\n", (base_int_t)( a ), (base_int_t)( b ) );                                                                                                     \
        }                                                                                                                                                                                              \
    } while ( 0 )
#define BASE_ASSERT_UINT( a, b )                                                                                                                                                                       \
    do {                                                                                                                                                                                               \
        if ( ( a ) != ( b ) ) {                                                                                                                                                                        \
            PRINT_INFO();                                                                                                                                                                              \
            printf( "assertion a==b failed:\n" );                                                                                                                                                      \
            printf( "a %" BASE_PRIu " b %" BASE_PRIu "\n", (base_uint_t)( a ), (base_uint_t)( b ) );                                                                                                   \
        }                                                                                                                                                                                              \
        assert( ( a ) == ( b ) );                                                                                                                                                                      \
    } while ( 0 )

#define BASE_SILENT_ASSERT_UINT( a, b )                                                                                                                                                                \
    do {                                                                                                                                                                                               \
        if ( ( a ) != ( b ) ) {                                                                                                                                                                        \
            PRINT_INFO();                                                                                                                                                                              \
            printf( "assertion a==b failed:\n" );                                                                                                                                                      \
            printf( "a %" BASE_PRIu " b %" BASE_PRIu "\n", (base_iint_t)( a ), (base_uint_t)( b ) );                                                                                                   \
        }                                                                                                                                                                                              \
    } while ( 0 )
#endif

#ifdef NOPRINTMAT
#define PRINT_COLMAJ_MAT( ... ) ( (void)0 )
#else
#define PRINT_ARRAY( macro_array, macro_array_fmt, macro_array_len, msg )                                                                                                                              \
    do {                                                                                                                                                                                               \
        printf( "%s\n", msg );                                                                                                                                                                         \
        for ( base_int_t macro_i = 0; macro_i < ( macro_array_len ); ++macro_i ) {                                                                                                                     \
            printf( macro_array_fmt, macro_array[macro_i] );                                                                                                                                           \
        }                                                                                                                                                                                              \
    } while ( 0 )
#define PRINT_COLMAJ_MAT_1( macro_mat, macro_nr, macro_nc, msg )                                                                                                                                       \
    do {                                                                                                                                                                                               \
        printf( "%s\n", msg );                                                                                                                                                                         \
        printf( "view(mat)[0:%" BASE_PRId "][0:%" BASE_PRId "]\n", macro_nr, macro_nc );                                                                                                               \
        for ( base_int_t macro_i = 0; macro_i < ( macro_nr ); ++macro_i ) {                                                                                                                            \
            for ( base_int_t macro_j = 0; macro_j < ( macro_nc ); ++macro_j ) {                                                                                                                        \
                printf( "%f ", *( macro_mat + macro_i + ( macro_j * ( macro_nr ) ) ) );                                                                                                                \
            }                                                                                                                                                                                          \
            printf( "\n" );                                                                                                                                                                            \
        }                                                                                                                                                                                              \
    } while ( 0 )
#define PRINT_COLMAJ_MAT_2( macro_mat, macro_nr, macro_nc, msg, lda, start_nr, start_nc )                                                                                                              \
    do {                                                                                                                                                                                               \
        printf( "%s\n", msg );                                                                                                                                                                         \
        printf( " lda = %" BASE_PRId " view(mat)[%" BASE_PRId ":%" BASE_PRId "][%" BASE_PRId ":%" BASE_PRId "]\n", lda, start_nr, macro_nr, start_nc, macro_nc );                                      \
        for ( base_int_t macro_i = start_nr; macro_i < ( macro_nr ); ++macro_i ) {                                                                                                                     \
            for ( base_int_t macro_j = start_nc; macro_j < ( macro_nc ); ++macro_j ) {                                                                                                                 \
                printf( "%f ", *( macro_mat + macro_i + ( macro_j * ( lda ) ) ) );                                                                                                                     \
            }                                                                                                                                                                                          \
            printf( "\n" );                                                                                                                                                                            \
        }                                                                                                                                                                                              \
    } while ( 0 )
#define GET_PRINT_COLMAJ_MAT_MACRO( _1, _2, _3, _4, _5, _6, _7, NAME, ... ) NAME
#define PRINT_COLMAJ_MAT( ... ) GET_PRINT_COLMAJ_MAT_MACRO( __VA_ARGS__, PRINT_COLMAJ_MAT_2, INVALID, INVALID, PRINT_COLMAJ_MAT_1, UNUSED )( __VA_ARGS__ )

#define PRINT_ROWMAJ_MAT( macro_mat, macro_nr, macro_nc, msg )                                                                                                                                         \
    do {                                                                                                                                                                                               \
        printf( "%s\n", msg );                                                                                                                                                                         \
        for ( base_int_t macro_i = 0; macro_i < ( macro_nr ); ++macro_i ) {                                                                                                                            \
            for ( base_int_t macro_j = 0; macro_j < ( macro_nc ); ++macro_j ) {                                                                                                                        \
                printf( "%f ", *( macro_mat + macro_j + ( macro_i * ( macro_nc ) ) ) );                                                                                                                \
            }                                                                                                                                                                                          \
            printf( "\n" );                                                                                                                                                                            \
        }                                                                                                                                                                                              \
    } while ( 0 )
#endif

#define CHECK_COLMAJ_MAT_WITH_VEC( macro_mat, macro_vec, macro_nr, macro_nc, macro_ierr )                                                                                                              \
    do {                                                                                                                                                                                               \
        for ( base_int_t macro_i = 0; macro_i < ( macro_nc ); ++macro_i ) {                                                                                                                            \
            for ( base_int_t macro_j = 0; macro_j < ( macro_nr ); ++macro_j ) {                                                                                                                        \
                if ( macro_mat[macro_j + macro_i * ( macro_nr )] != macro_vec[macro_j] ) {                                                                                                             \
                    PRINT_INFO();                                                                                                                                                                      \
                    printf( "macro_mat[%zu] = %f != %f\n", macro_j, macro_mat[macro_j + macro_i * ( macro_nr )], macro_vec[macro_j] );                                                                 \
                    ( macro_ierr ) = ( macro_ierr ) + 1;                                                                                                                                               \
                }                                                                                                                                                                                      \
            }                                                                                                                                                                                          \
        }                                                                                                                                                                                              \
    } while ( 0 )

#define CHECK_ROWMAJ_MAT_WITH_VEC( macro_mat, macro_vec, macro_nr, macro_nc, macro_ierr )                                                                                                              \
    do {                                                                                                                                                                                               \
        for ( base_int_t macro_i = 0; macro_i < ( macro_nr ); ++macro_i ) {                                                                                                                            \
            for ( base_int_t macro_j = 0; macro_j < ( macro_nc ); ++macro_j ) {                                                                                                                        \
                if ( macro_mat[macro_j + macro_i * ( macro_nc )] != macro_vec[macro_i] ) {                                                                                                             \
                    PRINT_INFO();                                                                                                                                                                      \
                    printf( "macro_mat[%zu] = %f != %f\n", macro_j, macro_mat[macro_j + macro_i * ( macro_nc )], macro_vec[macro_j] );                                                                 \
                    ( macro_ierr ) = ( macro_ierr ) + 1;                                                                                                                                               \
                }                                                                                                                                                                                      \
            }                                                                                                                                                                                          \
        }                                                                                                                                                                                              \
    } while ( 0 )

#define FILL_COLMAJ_MAT_WITH_VEC( macro_mat, macro_vec, macro_nr, macro_nc )                                                                                                                           \
    do {                                                                                                                                                                                               \
        for ( base_int_t macro_i = 0; macro_i < ( macro_nc ); ++macro_i ) {                                                                                                                            \
            for ( base_int_t macro_j = 0; macro_j < ( macro_nr ); ++macro_j ) {                                                                                                                        \
                *( macro_mat + macro_j + ( macro_i * ( macro_nr ) ) ) = *( macro_vec + macro_j );                                                                                                      \
            }                                                                                                                                                                                          \
        }                                                                                                                                                                                              \
    } while ( 0 )

#define FILL_ROWMAJ_MAT_WITH_VEC( macro_mat, macro_vec, macro_nr, macro_nc )                                                                                                                           \
    do {                                                                                                                                                                                               \
        for ( base_int_t macro_i = 0; macro_i < ( macro_nr ); ++macro_i ) {                                                                                                                            \
            for ( base_int_t macro_j = 0; macro_j < ( macro_nc ); ++macro_j ) {                                                                                                                        \
                *( macro_mat + macro_j + ( macro_i * ( macro_nc ) ) ) = *( macro_vec + macro_i );                                                                                                      \
            }                                                                                                                                                                                          \
        }                                                                                                                                                                                              \
    } while ( 0 )

// Source - https://stackoverflow.com/a/77881417
// Posted by Lundin, modified by community. See post 'Timeline' for change history
// Retrieved 2026-11-01, License - CC BY-SA 4.0

#define IS_ARRAY( T ) _Generic( &( T ), typeof ( *T )( * )[]: true, default: false )

// Source - https://stackoverflow.com/a/77939604
// Posted by Lundin
// Retrieved 2026-11-01, License - CC BY-SA 4.0

#define IS_POINTER( P ) _Generic( &( typeof( P ) ){ 0 }, typeof( *P ) **: 1, default: 0 )

#endif  //__BASE_HEADER_H__
