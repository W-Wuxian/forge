#ifndef __RGS_H__
#define __RGS_H__

#include "sketch.h"

#if !defined( WITH_MPI )
#define MPI_Comm void *
#define MPI_SUCCESS 0
// insde TSQR MPI_Allgather is switched for dlacpy
//((void)0)
#define MPI_Comm_rank( ... ) 0
#define MPI_Comm_size( ... ) 0
#define MPI_Allgather( ... ) 0
#define MPI_Allreduce( ... ) 0
#endif

#define IDX_USER_BLOCK_SIZE IDX_SKETCH_IPARAM_LEN /**< index for the user block size */                         // NBLOCKS
#define IDX_LSS_ALG ( IDX_SKETCH_IPARAM_LEN + 1 ) /**< index for the Least-Squares Solver */                    // WLSS
#define IDX_NUMBER_REORTHO_LSS ( IDX_SKETCH_IPARAM_LEN + 2 ) /**< index for the number of iteration/reortho */  // ILSS
#define IDX_NUMBER_REORTHO_SKETCHED_CHOLQR                                                                                                                                                             \
    ( IDX_SKETCH_IPARAM_LEN + 3 ) /**< index for the number (ll) of reorthogonalization for Sketch CholQR if 0 then RBGS uses L2QR-Sketch-CholQR else it uses Sketch-CholQR ll times */  // ISKCHL
#define IDX_RGS_IPARAM_LEN ( IDX_SKETCH_IPARAM_LEN + 4 ) /**< Actual Length of the RGS iparam array */  // RGS_iparam_length

/**
 * @enum base_RGS_LSS_Alg_t
 * @brief RGS Least-Squares solver (LSS) algorithm
 * @details Allows the user to select the LSS algorithm
 * used to solve the Inner LLS in the RGS process
 */
typedef enum {
    RGS_CGS,  /** Classical Gram-Schmidt with l re-ortthogonalization */
    RGS_BMGS, /** Block Modified Gram-Schmidt with l re-ortthogonalization */
    RGS_HH,   /** Householder solver i.e gels QR */
    RGS_BCGLS /** Block Conjugate Gradient Least Squares */
} base_rgs_lss_alg_e;

/**
 * \struct base_rgs_t
 * \brief The RGS data structure in double precision
 * \details This structure describes datas with different characteristics that will be used by
 * the RGS solver:
 *            - data dimensions
 *            - block sizes
 *            - current RGS iteration
 *            - sketch structure
 *            - type of the Least-Squares solver
 *            - Pointer to the user memory
 *            - memory needed by RGS
 */
typedef struct {
    base_sketch_t sketch_data; /**< sketch struct */

    // data_work main memory for: tmpwork PSke SSke Rfac Qfac tmpTSQR
    double *data_work;  //*workd;
    // ptr
    double *tmp_data_work;  // *tmpwork; /**< nrQ+Kproj+Kproj*nD (Dynamic) trash memory to deal with copy */
    double *pske;           // *PSke;    /**< Output Sketch of W (pi = Theta wi) (Kproj x ncols) (Dynamic) */
    double *sske;           // *SSke;    /**< Output Sketch of Q (Kproj x ncols) (Dynamic) */
    // double* swork;
    double *rfac;           //*Rfac;    /**< Output R-factor (Solving least-squares pb) (ncols x ncols) (Dynamic) */
    double *qfac;           //*Qfac;    /**< Output Q-factor (nIn x ncols) (Dynamic) */
    double *tmp_data_tsqr;  //*tmpTSQR; /**< tmp data for TSQR */
    // ptr
    double *data_need_facto;  //*W; /**< Input (nIn x ncols) (ptr to the user mem) data to be factorized */

    base_mem_t rgs_memory;
    double     rgs_flops;

    base_int_t len_tmp_data_work;     // lth_tmpwork; /**< length of tmpwork */
    base_int_t size_tmp_data_work;    // si_tmpwork;
    base_int_t sketch_dim;            // Kproj;                      /**< Dimension of the sketch */
    base_int_t nrows_data_in;         // nIn;                        /**< (local) size(In)[1] */
    base_int_t ncols_data_in;         // ncols; /**< size(In)[2] */  // 14-15
    base_int_t user_block_size;       // nblocks;                    /**< User block size (user input) */
    base_int_t current_block_size;    // blocks_size;                /**< The actual block size */
    base_int_t number_of_block;       // coef;                       /**< Number of nblocks */
    base_int_t remaining_block_size;  // remainder;                  /**< remaining block size */
    base_int_t size_data_work;        // size_workd;
    // base_int_t isalias; /**< Qfac as an alias */
    base_rgs_lss_alg_e lss_alg;  // wlss; /**< which Least-Squares Solver */

    int current_rgs_ite;                 // ite;  /**< actual rgs iteration */
    int number_reortho_lss;              // l;    /**< Number of re-orthogonalization computed by LSS */
    int number_reortho_sketched_cholqr;  // ll;   /**< Number or re-orthogonalization for Sketched CholQR */
    int rank;                            /**< MPI procs rank */
    int size;                            /**< Size of the MPI communicator */

    MPI_Comm comm; /**< The MPI communicator used by the user */
    /**
     * In parallel, Sketched matrices SkeIn, SkeQ and Rfac are fully stored on
     * each MPI procs. The Input and the Q-factor are stored row-major order(?).
     */
} base_rgs_t;

/**
 * \fn base_set_rgs_data
 * \brief Set RGS data with respect to the user iparam and MPI
 * \details Set sketch and memory used by RGS
 * \param[in, out] rgs_data pointer to the RGS struct
 * \param[in, out] iparam pointer to the integer parameter needed by RGS
 * \param[in] rank rank/ID of the MPI procs
 * \param[in] size Size of the MPI communicator
 * \param[in] comm The MPI communicator used
 */
void base_set_rgs_data( base_rgs_t *rgs_data, base_int_t *iparam, int rank, int size, MPI_Comm comm );

/**
 * @fn base_free_rgs_data
 * @brief Free memory created by RGS (and sketch)
 * @param[in, out] rgs_data pointer to the RGS struct
 */
void base_free_rgs_data( base_rgs_t *rgs_data );

/**
 * @fn base_get_data_rgs
 * @brief Advance the RGS memory with respect to the actual rgs iteration and
 * the input data
 * @details Also memset to zero tmpwork and actualize the blocks_size
 * @param[in, out] rgs_data pointer to the RGS struct
 * @param[in] rgs_ite actual RGS iteration
 * @param[in] data_need_facto pointer to the input data (the block to factorize/orthogonalize)
 */
void base_get_data_rgs( base_rgs_t *rgs_data, int rgs_ite, double *data_need_facto );

/**
 * @fn base_compute_rgs
 * @brief orthonormalized a Matrix-like object
 * @details using random project based on sketch
 * @param[in, out] rgs_data pointer to the RGS struct
 */
void base_compute_rgs( base_rgs_t *rgs_data );

/**
 * @fn _base_lss_rgs
 * @brief Private function
 * @details Least-Squares solver reduired by RGS
 * @param[in, out] rgs_data pointer to the RGS structure
 */
void _base_lss_rgs( base_rgs_t *rgs_data );

/**
 * @fn base_sketched_cholqr
 * @brief Skecthed CholQR factorization
 * @details Inputs :
 * - Q^{*}_{i} the updated ith-block of Qfac
 * - Theta the sketching matrix
 * Outputs :
 * - S^{*}_{i} the skecth of Q^{*}_{i}
 * - R_{i,i} the Cholesky factorization of (S^{*}_{i})^{T}S^{*}_{i}
 * - Q_{i} = Q^{*}_{i}(R_{i,i})^{-1}
 * @param[in,out] rgs_data - pointer to base_rgs_t data structure
 */
void base_sketched_cholqr( base_rgs_t *rgs_data );

/**
 * @fn base_l2qr_sketched_cholqr
 * @brief Inter-Block L2-QR + Sketched CholQR factorization
 * @details Inputs :
 * - Q^{*}_{i} the updated ith-block of Qfac
 * - Theta the sketching matrix
 * Outputs :
 * - Q_{i}R_{i,i} the QR factorization of Q^{*}_{i}
 * - S_{i} the skecth of Q_{i}
 * Improves the stability of the sketched CholQR by pre-processing the matrix Q^{*}_{i} with TSQR
 * @param[in,out] rgs_data - pointer to a RGS data structure
 */
void base_l2qr_sketched_cholqr( base_rgs_t *rgs_data );

int TSQR( MPI_Comm comm, base_int_t m, base_int_t n, double *A, double *Q, double *R, double *work );

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                RGS Utils and Errors                                                          */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Compute RGS flops
 *
 * @param rgs_data - pointer to a base_rgs_t data structure
 */
void base_flops_rgs( base_rgs_t *rgs_data );

/**
 * @brief Compute RGS memory
 *
 * @param rgs_memory - pointer to base_mem_t
 * @param sketch_memory - sketch_memory
 * @param mem_in_bytes - size_t
 */
void base_memory_rgs( base_mem_t *rgs_memory, base_mem_t sketch_memory, size_t mem_in_bytes );

/**
 * \fn base_backward_error
 * \brief Compute ||V-QR|| / ||V|| with the Frobenius norm
 * \param[in, out] V - pointer to matrix-object like, rewrite with (V - QR)
 * \param[in] m - size(V)[1]
 * \param[in] n - size(V)[2]
 * \param[in] Q - matrix-object like
 * \param[in] R - matrix-object like
 * \param[in] ldr - leading dimension of R
 * \return ||V-QR|| / ||V|| with the Frobenius norm
 */
double base_backward_error( double *V, base_int_t m, base_int_t n, double *Q, double *R, base_int_t ldr );

/**
 * \fn base_distbackward_error
 * \brief Compute ||V-QR|| / ||V|| with the Frobenius norm where V,Q are rowwise block 1-D partitioning
 * \param[in, out] V - pointer to matrix-object like, rewrite with (V - QR)
 * \param[in] m - size(V)[1]
 * \param[in] n - size(V)[2]
 * \param[in] Q - matrix-object like
 * \param[in] R - matrix-object like
 * \param[in] ldr - leading dimension of R
 * \param[in] comm - MPI communicator
 * \return ||V-QR|| / ||V|| with the Frobenius norm
 */
double base_distbackward_error( double *V, base_int_t m, base_int_t n, double *Q, double *R, base_int_t ldr, MPI_Comm comm );

/**
 * @brief Same as base_distbackward_error but V, Q are also know by column block
 *
 * @param V
 * @param m - size(V)[1]
 * @param n - size(V)[2]
 * @param ncols - size(Q)[2]
 * @param Q - Q factor
 * @param R - R factor
 * @param ldr - leading dimension of R
 * @param usrc - user choice for the matrix builder
 * @param lite - current iteration
 * @param nb - current number of columns
 * @param globalsize - global number of rows
 * @param TrV - trace (V)
 * @param comm - MPI communicator
 * @return ||V-QR|| / ||V|| with the Frobenius norm
 */
double base_blockdistbackward_error( double    *V,
                                     base_int_t m,
                                     base_int_t n,
                                     base_int_t ncols,
                                     double    *Q,
                                     double    *R,
                                     base_int_t ldr,
                                     int        usrc,
                                     int        lite,
                                     base_int_t nb,
                                     base_int_t globalsize,
                                     double     TrV,
                                     MPI_Comm   comm );

/**
 * \fn base_blockdisttrace
 * \brief Compute the Trace of a ATA where A is a column wise distributed block
 * \param[in] A - pointer to matrix-object like
 * \param[in] m - size(V)[1]
 * \param[in] n - size(V)[2]
 * \param[in] comm - MPI communicator
 * \return tr Trace(ATA)
 */
double base_blockdisttrace( double *A, base_int_t m, base_int_t n, MPI_Comm comm );

/**
 * \fn base_ortho_error
 * \brief Compute ||I-Q^TQ|| with the Frobenius norm
 * \param[in] Q - pointer to matrix-object like
 * \param[in] m - size(Q)[1]
 * \param[in] n - size(Q)[2]
 * \return ||I-Q^TQ|| with the Frobenius norm
 */
double base_ortho_error( double *Q, base_int_t m, base_int_t n );

/**
 * \fn base_cond_d
 * \brief Compute an approximated condition number of  with respect to the 2-norm
 * \details Using LAPACKE_dgsvd to compute the greater and smaller (smax and smin) singular values of
 * \param[in]  - pointer to matrix-object like
 * \param[in] m - size()[1]
 * \param[in] n - size()[2]
 * \return smax / smin
 */
double base_cond( double *, base_int_t m, base_int_t n );
/**
 * @brief reciprocal condition number
 *
 * @param  - pointer to matrix-object like
 * @param m - size()[1]
 * @param n - size()[2]
 * @return reciprocal condition number of
 */
double base_rcond( double *, base_int_t m, base_int_t n );

/**
 *
 * @fn distload
 * @brief Compute the local workload from a global workload
 * @details 1-D work load computation based on MPI rank and size
 * @param[in] rank MPI rank
 * @param[in] size MPI size
 * @param[in] work The global workload
 * @param[in, out] load The local (MPI rank based) workload
 *
 */
void distload( int rank, int size, base_int_t work, base_int_t *load );
void distloadidxi( int rank, int size, base_int_t work, base_int_t *load, base_int_t *ia1, base_int_t *ia2 );

/**
 * @brief Assert RGS errors
 * ||P-SR|| / ||R||
 * ||I-STS||
 * Cond(S)
 * Frobenius norm
 * @param rgs_data - pointer to a base_rgs_t data structure
 * @param n - number of vectors to check (n<=size(S)[2])
 */
void base_rgs_assert_error( base_rgs_t *rgs_data, base_int_t n );

/**
 * @brief Saving in a file ||P-SR||/||P||, ||W-QR||/||W||, ||I-STS|| and cond(S)
 *
 * @param rgs_data - pointer to a base_rgs_t data structure
 * @param W - pointer to a matrix-like onject
 * @param globalsize - global number of rows
 * @param nthreads - number of threads
 * @param usrc - user choice for the input matrix
 * @param isB - whether the matrix is build block by block
 * @param TrV - trace of the input matrix
 * @param MatName - matrix name
 * @param DirName - path to the directory used to store error file
 * @return int
 */
int base_rgs_saving_error( base_rgs_t *rgs_data, double *W, base_int_t globalsize, int nthreads, int usrc, int isB, double TrV, char *MatName, char *DirName );

#endif  //__RGS_H__
