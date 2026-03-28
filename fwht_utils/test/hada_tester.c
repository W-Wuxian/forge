// #include "hada.h"
#include "hada_tester.h"

#ifndef M
#define M 32
#endif

#ifndef N
#define N 1
#endif

base_int_t
test_trz( base_int_t n )
{
    base_int_t count = 0;
    trz( n, &count );
    return count;
}

base_uint_t
test_ilog2( base_int_t n )
{
    return base_ilog2( n );
}

bool
test_ispow2( base_int_t n )
{
    return base_ispow2( n );
}

base_int_t
test_n2p( base_int_t n )
{
    return base_n2p( n );
}