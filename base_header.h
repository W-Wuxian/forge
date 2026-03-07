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
// #include "mkl.h"
#include "base_datatype.h"

#define PRINT_INFO()                                                                               \
    printf( "In file:%s, function:%s() and line:%d ", __FILE__, __func__, __LINE__ )

#define BASE_ASSERT_INT( a, b )                                                                    \
    do {                                                                                           \
        PRINT_INFO();                                                                              \
        printf( "a %zu b %zu\n", (size_t)( a ), (size_t)( b ) );                                   \
        assert( ( a ) == ( b ) );                                                                  \
    } while ( 0 )

#endif  //__BASE_HEADER_H__
