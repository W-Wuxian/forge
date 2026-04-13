#ifndef __BASE_HEADER_H__
#define __BASE_HEADER_H__

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <inttypes.h>
#include <cblas.h>
#include "fftw3.h"
// #include "mkl.h"
#include "base_datatype.h"

#define PRINT_INFO() printf( "In file:%s, function:%s() and line:%d ", __FILE__, __FUNCTION__, __LINE__ )

#define BASE_ASSERT_ISNOTNULL( a )                                                                                                                                                                     \
    do {                                                                                                                                                                                               \
        if ( ( a ) == NULL ) {                                                                                                                                                                         \
            PRINT_INFO();                                                                                                                                                                              \
            printf( "assertion a != NULL failed:\n" );                                                                                                                                                 \
        }                                                                                                                                                                                              \
        assert( ( a ) != NULL );                                                                                                                                                                       \
    } while ( 0 )

#ifdef NOASSERTINT
#define BASE_ASSERT_INT( a, b ) ( (void)0 )
#define BASE_SILENT_ASSERT_INT( a, b ) ( (void)0 )
#else
#define BASE_ASSERT_INT( a, b )                                                                                                                                                                        \
    do {                                                                                                                                                                                               \
        if ( ( a ) != ( b ) ) {                                                                                                                                                                        \
            PRINT_INFO();                                                                                                                                                                              \
            printf( "assertion a==b failed:\n" );                                                                                                                                                      \
            printf( "a %zu b %zu\n", (size_t)( a ), (size_t)( b ) );                                                                                                                                   \
        }                                                                                                                                                                                              \
        assert( ( a ) == ( b ) );                                                                                                                                                                      \
    } while ( 0 )

#define BASE_SILENT_ASSERT_INT( a, b )                                                                                                                                                                 \
    do {                                                                                                                                                                                               \
        if ( ( a ) != ( b ) ) {                                                                                                                                                                        \
            PRINT_INFO();                                                                                                                                                                              \
            printf( "assertion a==b failed:\n" );                                                                                                                                                      \
            printf( "a %zu b %zu\n", (size_t)( a ), (size_t)( b ) );                                                                                                                                   \
        }                                                                                                                                                                                              \
    } while ( 0 )
#endif

#ifdef NOPRINTMAT
#define PRINT_COLMAJ_MAT( macro_mat, macro_nr, macro_nc, msg ) ( (void)0 )
#define PRINT_ROWMAJ_MAT( macro_mat, macro_nr, macro_nc, msg ) ( (void)0 )
#else
#define PRINT_COLMAJ_MAT( macro_mat, macro_nr, macro_nc, msg )                                                                                                                                         \
    do {                                                                                                                                                                                               \
        printf( "%s\n", msg );                                                                                                                                                                         \
        for ( base_int_t macro_i = 0; macro_i < ( macro_nr ); ++macro_i ) {                                                                                                                            \
            for ( base_int_t macro_j = 0; macro_j < ( macro_nc ); ++macro_j ) {                                                                                                                        \
                printf( "%f ", *( macro_mat + macro_i + ( macro_j * ( macro_nr ) ) ) );                                                                                                                \
            }                                                                                                                                                                                          \
            printf( "\n" );                                                                                                                                                                            \
        }                                                                                                                                                                                              \
    } while ( 0 )

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

#ifdef _WIN32
#define SPEALLOC( ptr, align, size ) ( ( ( ptr ) = _aligned_malloc( ( size ), ( align ) ) ) == NULL ? -1 : 0 )
#elif defined( DUSE_POSIX_MEMALIGN )
#define SPEALLOC( ptr, align, size ) posix_memalign( (void **)&( ptr ), ( align ), ( size ) )
#elif defined( DUSE_MKL_MALLOC )
#define SPEALLOC( ptr, align, size ) ( ( ( ptr ) = mkl_malloc( ( size ), ( align ) ) ) == NULL ? -1 : 0 )
#else
#define SPEALLOC( ptr, align, size ) ( ( ( ptr ) = malloc( size ) ) == NULL ? -1 : 0 )
#endif

#endif  //__BASE_HEADER_H__
