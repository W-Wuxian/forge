#ifndef __FWHT_H__
#define __FWHT_H__

#include "hada.h"

/**
 * \fn _base_rotatedata
 * \brief private function
 * \details Orthogonal transform for data of type double
 * Formulae: a = c +/- b
 * \param[in,  out] data pointer to the 1D Array or Vector-like object (ColMajor flat profil) to be
 * transform \param[in] c1 index of the element in array to rotate with respect to c2 \param[in] c2
 * index of the element in array to rotate with respect to c1
 */
void _base_rotatedata( double *data, base_uint_t c1, base_uint_t c2 );

/**
 * \fn base_dummy_fwht
 * \brief Fast Walsh-Hadamard transform
 * \details Double precision version, without normalization
 * \param[in, out] data pointer to the array to be transform
 * \param[in] n length of data
 */
void base_dummy_fwht( double *data, base_uint_t n );

#endif  //__FWHT_H__
