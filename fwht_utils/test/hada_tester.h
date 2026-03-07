#ifndef __HADA_TESTER_H__
#define __HADA_TESTER_H__

#include "hada.h"

base_int_t test_trz( base_int_t n );

void test_getdimshowmany( view_t *In, base_int_t *Tdims, base_int_t *Thow, base_int_t *hdim );

void test_gethdims( base_int_t *Tdims, base_int_t *Hdims );

base_uint_t test_ilog2( base_int_t n );

bool test_ispow2( base_int_t n );

base_int_t test_n2p( base_int_t n );

#endif  //__HADA_TESTER_H__
