#ifndef __FWHT_H__
#define __FWHT_H__

#include "hada.h"
// #include <cblas.h>

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
 * among the global number of rows.
 * Assuming data is Col-Major
 * so ridx1 is "row index 1" i.e row index to data for x mapping
 * and ridx2 is "row index 2" i.e row index to data for y mapping
 * Since data is Col-Major, ridx1 and ridx2 values are in 0:nrows-1 (direct mapping)
 * and then incx, incy (increments for elements of x, y) are both equals to nrows.
 * This also means that the number of elements in vector x, y is ncols.
 * \param[in,  out] data pointer to the 2D array or Matrix-like
 * object (ColMajor flat profil) to be transform
 * \param[in] nrows nrows size(data)[1] \param[in]
 * ncols size(data)[2]
 * \param[in] ridx1 index of the element in array to rotate with respect to
 * ridx2
 * \param[in] ridx2 index of the element in array to rotate with respect to ridx1
 */
void _base_rotatedata_mat( double *data, base_int_t nrows, base_int_t ncols, base_uint_t ridx1, base_uint_t ridx2 );

// v2 data.view -> copy -> buffer -> cblas_drotm -> copy -> data
void _base_rotatedata_mat_v2( double *data, double *buffer, base_int_t nrows, base_int_t ncols, base_uint_t ridx1, base_uint_t ridx2 );
// v3 data.view -> copy -> buffer -> cblas_drotm on contiguous elements -> copy -> dat
void _base_rotatedata_mat_v3( double *data, double *buffer, base_int_t nrows, base_int_t ncols, base_uint_t ridx1, base_uint_t ridx2 );

/**
 * \enum _base_rotatedata_mat_rmaj
 * \brief private function
 * \details Orthogonal transform, data is a flat object of length n := nrows * ncols
 * who can be view as a matrix object-like nrows x ncols. ridx1 and ridx2 are the rows
 * index in the matrix view. In parallel, nrows refers for each process to their local workload
 * among the global number of rows.
 * Assuming data is Row-Major
 * so ridx1 is "row index 1" i.e row index to data for x mapping
 * and ridx2 is "row index 2" i.e row index to data for y mapping
 * Since data is Row-Major, ridx1 and ridx2 values are in 0:nrows-1 * offset
 * due to row-major format. offset is equal to ncols.
 * and then incx, incy (increments for elements of x, y) are both equals to one.
 * This also means that the number of elements in vector x, y is ncols.
 * \param[in,  out] data pointer to the 2D array or Matrix-like
 * object (ColMajor flat profil) to be transform
 * \param[in] nrows nrows size(data)[1] \param[in]
 * ncols size(data)[2]
 * \param[in] ridx1 index of the element in array to rotate with respect to
 * ridx2
 * \param[in] ridx2 index of the element in array to rotate with respect to ridx1
 */
void _base_rotatedata_mat_rmaj( double *data, base_int_t nrows, base_int_t ncols, base_uint_t ridx1, base_uint_t ridx2 );

void _base_rotatedata_mat_rmaj_v2( double *data, base_int_t nrows, base_int_t ncols, base_uint_t ridx1, base_uint_t ridx2 );

void _base_rotatedata_mat_rmaj_v3( double *__restrict__ data, base_int_t nrows, base_int_t ncols, base_uint_t ridx1, base_uint_t ridx2 );

void _base_rotatedata_mat_rmaj_v4( double *__restrict__ data, base_int_t nrows, base_int_t ncols, base_uint_t ridx1, base_uint_t ridx2 );

void _base_rotatedata_mat_rmaj_v5( double *__restrict__ data, base_int_t nrows, base_int_t ncols, base_uint_t ridx1, base_uint_t ridx2 );

void base_fwht_mat( double *data, base_int_t n, base_int_t ncols );

void base_fwht_mat_v3( double *data, double *buffer, base_int_t n, base_int_t ncols );

void base_fwht_mat_rmaj( double *data, base_int_t n, base_int_t ncols );

void base_fwht_mat_rmaj_v2( double *data, base_int_t n, base_int_t ncols );

void base_fwht_mat_rmaj_v3( double *__restrict__ data, base_int_t n, base_int_t ncols );

void base_fwht_mat_rmaj_v4( double *__restrict__ data, base_int_t n, base_int_t ncols );

void base_fwht_mat_rmaj_v5( double *__restrict__ data, base_int_t nrows, base_int_t ncols );

/**
 * @brief base_SetFFTW
 *
 * @param Hadaplan pointer to fftw_plan
 * @param vIn pointer to viewt_t
 * @param FT int 0: FFTW_ESTIMATE 1: FFTW_MEASURE
 * @param In pointer to Input data to be transform (out-of-place)
 * @param Out pointer to Output data
 */
void base_SetFFTW( fftw_plan *Hadaplan, view_t *vIn, int FT, double *In, double *Out );

/**
 * @brief base_FreeFFTW
 * @details Free memory used by FFTW
 * @param Hadaplan fftw_plan
 */
void base_FreeFFTW( fftw_plan *Hadaplan );

/**
 * @brief FWHT flops
 * Formulae: m*n*log(m)
 * @param m
 * @param n
 * @return size_t
 */
size_t base_flops_FWHT( base_int_t m, base_int_t n );

#endif  //__FWHT_H__
