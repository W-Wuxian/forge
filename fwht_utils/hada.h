#ifndef HADA_H
#define HADA_H

#include "base_header.h"

#define _DDIMS ( (base_int_t)3 )

typedef struct {
    base_int_t m;   /**< A is m x n ColMaj */
    base_int_t n;   /**< A is m x n ColMaj */
    base_int_t st1; /**< A is m x n ColMaj, then st1 = 1 and st2 = m */
    base_int_t st2; /**< A is m x n ColMaj, then st1 = 1 and st2 = m */
} view_t;

/**
 * @brief Trailing zeros
 *
 * @param n
 * @param count
 */
void trz( base_int_t n, base_int_t *count );

/**
 * @brief Set data for FFTW
 *
 * @param[Out] In
 * @param[Out] Tdims
 * @param[Out] Thow
 * @param[Out] hdim
 */
void getdimshowmany( view_t *In, base_int_t *Tdims, base_int_t *Thow, base_int_t *hdim );

/**
 * @brief Set data for FFTW (should be eqv to hadamardize from Hadamard.jl)
 *
 * @param[In] Tdims
 * @param[Out] Hdims _DDIMS x hdims
 */
void gethdims( base_int_t *Tdims, base_int_t *Hdims );

/**
 * @brief Show Hdims, a matrix-object as it will be used to fill fftw_iodim
 *
 * @param Hdims matrix
 * @param ncols
 */
void showparsedhdims( base_int_t *Hdims, base_int_t ncols );

/**
 * \fn base_ilog2
 * \brief Minus 1 Integer logarithm base 2 (I think...)
 * \param[in] n
 * \return unsigned p, such that n=2^(p-1)
 */
base_uint_t base_ilog2( base_int_t n );

/**
 * \fn base_ispow2
 * \brief Is a power of 2
 * \param[in] n
 * \return 1 if n = 2^p
 * \return 0 else
 */
bool base_ispow2( base_int_t n );

/**
 * \fn base_n2p
 * \brief Compute the next power of 2
 * \param[in] n
 * \return int value
 */
base_int_t base_n2p( base_int_t n );

#endif