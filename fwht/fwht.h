#ifndef __FWHT_H__
#define __FWHT_H__

#include "hada.h"
//#include <cblas.h>

/**
 * \fn _base_rotatedata
 * \brief private function
 * \details Orthogonal transform for data of type double
 * Formulae: a = c +/- b
 * \param[in,  out] data pointer to the 1D Array or Vector-like object (ColMajor flat profil) to be
 * transform \param[in] ridx1 index of the element in array to rotate with respect to ridx2
 * \param[in] ridx2 index of the element in array to rotate with respect to ridx1
 */
void _base_rotatedata( double *data, base_uint_t ridx1, base_uint_t ridx2 );

/**
 * \fn base_dummy_fwht
 * \brief Fast Walsh-Hadamard transform
 * \details Double precision version, without normalization
 * \param[in, out] data pointer to the array to be transform
 * \param[in] n length of data
 */
void base_dummy_fwht( double *data, base_uint_t n );

/**
 * \enum _base_rotatedata_mat
 * \brief private function
 * \details Orthogonal transform, data is a flat object of length n := nrows * ncols
 * who can be view as a matrix object-like nrows x ncols. ridx1 and ridx2 are the rows
 * index in the matrix view. In parallel, nrows refers for each process to their local workload
 * among the global number of rows. \param[in,  out] data pointer to the 2D array or Matrix-like
 * object (ColMajor flat profil) to be transform \param[in] nrows nrows size(data)[1] \param[in]
 * ncols size(data)[2] \param[in] ridx1 index of the element in array to rotate with respect to
 * ridx2 \param[in] ridx2 index of the element in array to rotate with respect to ridx1
 */
void _base_rotatedata_mat( double      *data,
                           base_int_t  nrows,
                           base_int_t  ncols,
                           base_uint_t ridx1,
                           base_uint_t ridx2 );

#endif  //__FWHT_H__
