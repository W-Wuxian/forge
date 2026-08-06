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
    double           *data_out;          /**< Pointer (double prec) to the user memory for the Sketch of data_in */
    double           *data_work;         /**< Dynamic array (nswork x ncols_data_in) used to resize data_in at a power of two with zero-padding */
    size_t            memMB;             /**< memory allocated by SKETCH in MB */
    size_t            flops;             /**< SKETCH flops */
    base_int_t       *rademacher_array;  /**< Dynamic array used to store the Rademacher array needed by SRHT algo */
    base_int_t       *permutation_array; /**< Dynamic array used to store the Permutation array needed by SRHT algo */
    base_sketch_alg_e sketch_alg;        /**< SRHT_CFWHT or SRHT_FFTW or GAUSS */
    base_int_t        nrows_data_in;     /**< (local) size(data_in)[1]  */
    base_int_t        ncols_data_in;     /**< size(data_in)[2] */
    base_int_t        sketch_dim;        /**< Sketch  dimension*/
    base_int_t        nDr;               /**<  length of the rademacher right array, computed internally */
    base_int_t        nDl;               /**<  length of the rademacher left array, computed internally */
    base_int_t        nrows_data_work;   /**< size(data_work)[1] = nswork, and nswork is the next power of 2 from nrows_data_in if wsketch != GAUSS else nrows_data_in, computed internally */
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

/**
 * @brief Set internal flags based on user parameters
 * and initialize memory for permutation, rademacher arrays
 *
 * @param sketch_data  - pointer to base_sketch_t data structure
 * @param iparam - pointer to sketch parameters
 * @param rank - MPI process ID
 * @param size - MPI communicator size
 */
void base_init_sketch_data( base_sketch_t *sketch_data, base_int_t *iparam, int rank, int size );

/**
 * \fn base_set_sketch_data
 * \brief Memory allocation and initialization for swork array (sketch)
 *
 * \param[in, out] sketch_data - pointer base_sketch_t struct
 * \param[in] rank - MPI procs rank
 * \param[in] size - number of MPI process
 */
void base_set_sketch_data( base_sketch_t *sketch_data, int rank, int size );

/**
 * \fn base_free_sketch_data
 * \brief Free memory allocated by base_Set_d_SKETCH.
 * \param[in, out] sketch_data - pointer to base_sketch_t struct
 */
void base_free_sketch_data( base_sketch_t *sketch_data );

/**
 * \fn base_getdata_sketch_data
 * \brief Used to share pointers, public function
 * \details Update sketch pointers with the user memory, update sketch ncols_data_in
 * \param[in, out] sketch_data - pointer to base_sketch_t struct
 * \param[in] ncols_data_in - number of columns of data_in
 * \param[in] data_in - pointer to the input data
 * \param[in] data_out - pointer to the output data
 */
void base_getdata_sketch_data( base_sketch_t *sketch_data, const base_int_t ncols_data_in, double *data_in, double *data_out );

/**
 * \fn base_compute_sketch
 * \brief double precision version of the srth with a 1D input data, public function
 * \details  Compute SKETCH of an input vector
 * nrows_data_in number of rows of data_in, nrows_data_in as to be a power of 2 if not try zero-padding before calling this function
 * data_out the skecth of data_in
 * Kproj dimension of the sketch, with Kproj << nrows_data_in
 * D rademacher array
 * nD length of Dr (D[0:nDr-1]==Dr)
 * Perm permutation array
 * swork working array for fwht()
 * nswork length of swork
 * scale factor (1 / sqrt(Kproj))
 * \param[in,out] sketch_data - pointer to base_sketch_t struct
 */
void base_compute_sketch( base_sketch_t *sketch_data );

#endif  //__SKETCH_H__
