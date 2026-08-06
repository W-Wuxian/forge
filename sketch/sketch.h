#ifndef __SKETCH_H__
#define __SKETCH_H__

#include "base_fwht.h"

typedef enum { SRHT_CFWHT, SRHT_FFTW, SRHT_HADI_FWHT, GAUSS } base_sketch_alg_e;

/**
 * \struct base_sketch_t
 * \brief The SKETCH data structure in double precision
 * \details This structure describes datas with different characteristics that will be used by
 * the SKETCH solver:
 *              - data dimensions
 *              - scaling factor
 *              - Pointer to the user memory
 *              - SKETCH memory
 * SRHT uses FWHT algo
 */
typedef struct {
    fftw_plan         Hadaplan;          /**< FFTW Hadamard plan */
    double            scale;             /**< scaling  factor needed by SKETCH algo, computed with base_set_sketch */
    double           *data_in;           /**< Pointer (double prec) to the user memory for the data to be Sketch */
    double           *data_out;          /**< Pointer (double prec) to the user memory for the Sketch of In */
    double           *data_work;         /**< Dynamic array (nswork x ncols) used to resize In at a power of two with zero-padding */
    size_t            memMB;             /**< memory allocated by SKETCH in MB */
    size_t            flops;             /**< SKETCH flops */
    base_int_t       *rademacher_array;  /**< Dynamic array used to store the Rademacher array needed by SRHT algo */
    base_int_t       *permutation_array; /**< Dynamic array used to store the Permutation array needed by SRHT algo */
    base_sketch_alg_e sketch_alg;        /**< SRHT_CFWHT or SRHT_FFTW or GAUSS */
    base_int_t        nrows_data_in;     /**< (local) size(In)[1]  */
    base_int_t        ncols_data_in;     /**< size(In)[2] */
    base_int_t        sketch_dim;        /**< Sketch  dimension*/
    base_int_t        nDr;               /**<  length of the rademacher right array, computed internally */
    base_int_t        nDl;               /**<  length of the rademacher left array, computed internally */
    base_int_t        nrows_data_work;   /**< size(data_work)[1] = nswork, and nswork is the next power of 2 from nIn if wsketch != GAUSS else nIn, computed internally */
} base_sketch_t;

/**
 * \fn _base_create_random_permutation
 * \brief Private function
 * \details With a fixed seed, from an input size input_len, it creates a random permutation array data_out
 * of size len_data_out <= input_len.
 * Ex: for(i = 0; i < len_data_out; ++i)cblas_dcopy(1, A[data_out[i]], 1, B[i], 1);
 * \param[in] input_len - Length of the array on wich we want to proceed a permutation
 * \param[in, out] data_out - Pointer to a vector-like object storing permutation
 * \param[in] len_data_out - Dimension of the permutation
 */
void _base_create_random_permutation( base_int_t input_len, base_int_t *data_out, base_int_t len_data_out );

#endif  //__SKETCH_H__
