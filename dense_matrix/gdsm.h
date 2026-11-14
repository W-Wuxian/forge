#ifndef __GDSM_H__
#define __GDSM_H__

// Generate DenSe Matrix
#include "base_header.h"

#define BASE_MATCREATE( mat, init_func, ... ) init_func( ( mat ), __VA_ARGS__ )

typedef enum { BASE_ROWMAJOR, BASE_COLMAJOR } base_matrix_layout_e;
typedef enum { BASE_MAT_INIT_SYNTETIC, BASE_MAT_INIT_LINEAR, BASE_MAT_INIT_RANGE_RANDOM } base_matrix_init_method_e;
/* f_{mu}(x)= ( sin( 10(mu+x) ) ) / ( cos( 100(mu-x) ) + 1.1 ) for x in [0,1] for parameter mu in [0,1]
function's domain is discretized with n=10^6 evenly spaces points x_i
parameter set is discretized with m=300 venly spaces points mu_j.
W
matrix [W]_{i,j} = f_{mu_j}(x_i), 1 <= i <= n, 1 <= j <= m
https://arxiv.org/pdf/2011.05090 Randomized Gram-Schmidt process with application to GMRES
Oleg Balabanov and Laura Grigori */

/**
 * @brief syntetic function
 * Formulae: f_{mu}(x) = ( sin( (double)10 * ( mu + x ) ) ) / ( cos( (double)100 * ( mu - x ) ) + 1.1 )
 *
 * @param x variable
 * @param mu parameter mu << x
 * @return double
 */
static inline double
dorthobasis_sincos( double x, double mu )
{
    return ( sin( (double)10 * ( mu + x ) ) ) / ( cos( (double)100 * ( mu - x ) ) + 1.1 );
}

/**
 * @brief Initialize a dense Matrix with a syntetic function
 *
 * @param mat
 * @param n number of rows
 * @param m number of columns
 * @param layout BASE_COLMAJOR or BASE_ROWMAJOR
 */
void dinitorthobasis_sincos( double *mat, base_int_t n, base_int_t m, base_matrix_layout_e layout );

/**
 * @brief Initialize a dense Matrix with linear formulae
 * P(i,j) = i*k+j
 *
 * @param mat
 * @param nr number of rows
 * @param nc number of columns
 * @param layout BASE_COLMAJOR or BASE_ROWMAJOR
 */
void dinitlinear( double *mat, base_int_t nr, base_int_t nc, base_matrix_layout_e layout );

/**
 * @brief Initialize a dense Matrix with random values from range
 *
 * @param mat
 * @param nele nrows x ncols
 * @param min
 * @param max
 * @param se if se > 0 used it for srand else srand seed is based on time
 */
void dinitwrandrange( double *mat, base_int_t nele, double min, double max, base_uint_t se );

/**
 * @brief Initialize a dense Matrix with lapacke_dlarnv
 *
 * @param MAT
 * @param rank
 * @param localsize rank's Workload
 * @param ncols
 * @param nb fill nb colunms
 * @param cite current filling iteration
 */
void dinitlarnv( double *mat, int rank, base_int_t localsize, base_int_t ncols, base_int_t nb, int cite );

/**
 * @brief Create a dense matrix based on a Matrix from Matrix Market
 * so sparse to dense or dense to dense, rhs to dense
 * from mmio dense is eqv to array and array for them is array1D
 * if multiple rhs? probably rhs1|...|rhsN
 *
 * @param mat return a pointer to allocated memory
 * @param fmm path to MatrixMarket file
 * @param layout BASE_COLMAJOR or BASE_ROWMAJOR
 */
void dinitmmiodense( double **mat, int *m, int *n, int *nz, char fmm[], base_matrix_layout_e layout );

#endif  //__GDSM_H__
