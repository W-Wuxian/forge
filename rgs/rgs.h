#ifndef __RGS_H__
#define __RGS_H__

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
} base_rgs_lss_alg_t;

#endif  //__RGS_H__
