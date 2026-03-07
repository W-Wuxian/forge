
// #include <fftw3.h>
// #include "mkl.h"

// reference implementation :
// https://github.com/JuliaMath/Hadamard.jl/blob/master/src/Hadamard.jl
// https://github.com/JuliaMath/FFTW.jl/blob/master/src/fft.jl

#include "hada.h"

void
trz( base_int_t n, base_int_t *count )
{
    *count = 0;
    while ( n > 0 && ( n & 1 ) == 0 ) {
        *count += 1;
        n = n >> 1;
    }
}

void
getdimshowmany( view_t *vIn, base_int_t *Tdims, base_int_t *Thow, base_int_t *hdim )
{
    // reg = int[region...]
    // implicit definition :: base_int_t reg    = 1;        /**< region = reg === 1 */
    Tdims[_DDIMS - _DDIMS] = vIn->m;   /**< size(In)[reg] = m (In is m x n) */
    Tdims[_DDIMS - 2]      = vIn->st1; /**< strides(In)[reg] = (1,m)[reg] */
    Tdims[_DDIMS - 1] =
        vIn->st1; /**< strides(Out)[reg] = (1,m)[reg] we assume that In and Out have EQV view_t */
    // oreg = [1:ndims(In)]
    // implicit definition :: base_int_t oreg       = 2;        /**< zeroed oreg[reg] and filter
    // oreg <- (oreg[]>0) */
    Thow[_DDIMS - _DDIMS] = vIn->n;   /**< size(In)[oreg] = n (In is m x n) */
    Thow[_DDIMS - 2]      = vIn->st2; /**< strides(In)[reg] = (1,m)[oreg] */
    Thow[_DDIMS - 1]      = vIn->st2; /**< strides(Out)[reg] = (1,m)[oreg] */
    // hdim
    base_int_t nh = Tdims[_DDIMS - _DDIMS];
    trz( nh, hdim );
}

void
gethdims( base_int_t *Tdims, base_int_t *Hdims )
{
    base_int_t ntot = 0;
    base_int_t k1 = 0, k2 = 0;
    k1 = Tdims[_DDIMS - _DDIMS];
    trz( k1, &ntot );
    k1 = 0;
    k2 = ntot;
    printf( "Tdims[_DDIMS - _DDIMS] %zu,k1 %zu, k2 %zu\n", Tdims[_DDIMS - _DDIMS], k1, k2 );
    base_int_t is = Tdims[_DDIMS - 2];
    base_int_t os = Tdims[_DDIMS - 1];
    for ( base_int_t k = k1; k < k2; ++k ) {
        printf( "k %zu _DDIMS * k %zu is %zu os %zu\n", k, _DDIMS * k, is, os );
        Hdims[( _DDIMS - _DDIMS ) + _DDIMS * k] = 2;
        Hdims[( _DDIMS - 2 ) + _DDIMS * k]      = is;
        Hdims[( _DDIMS - 1 ) + _DDIMS * k]      = os;
        is *= 2;
        os *= 2;
    }
    // Hdims is _DDims x ntot ; C is Row-Major
    // Hdims has to be parse to fftw_iodim
    // parser hes to be:
    // This function transforms each individual power-of-two dimension n into the corresponding
    // log2(n) DFT dimensions. showparsedhdims can be used to do it
}

void
showparsedhdims( base_int_t *Hdims, base_int_t ncols )
{
    for ( int j = 0; j < _DDIMS; ++j ) {
        for ( int i = 0; i < ncols; ++i ) {
            printf( "%zu\t", Hdims[j + i * _DDIMS] );
        }
        printf( "\n" );
    }
}

base_uint_t
base_ilog2( base_int_t n )
{
    base_uint_t l;
    for ( l = 0; n; n >>= 1, ++l )
        ;
    return l;
}

bool
base_ispow2( base_int_t n )
{
    return ( n > 1 ) && ( ( n & ( n - 1 ) ) == 0 );
}

base_int_t
base_n2p( base_int_t n )
{
    base_uint_t l  = n;
    base_uint_t l2 = base_ilog2( l ) - base_ispow2( l );
    base_uint_t N  = 1 << l2;
    return (base_int_t)N;
}