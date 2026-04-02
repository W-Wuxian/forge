#include "hada.h"

#ifndef HADA_SHOWCASE_ROW_M
#define HADA_SHOWCASE_ROW_M 16
#endif

#ifndef HADA_SHOWCASE_ROW_N
#define HADA_SHOWCASE_ROW_N 1
#endif

int
main()
{
    base_int_t ierr = 0;
    view_t     vIn  = {
             .m = HADA_SHOWCASE_ROW_M, .n = HADA_SHOWCASE_ROW_N, .st1 = 1, .st2 = HADA_SHOWCASE_ROW_M
    };
    printf( "m %zu n %zu st1 %zu st2 %zu\n", vIn.m, vIn.n, vIn.st1, vIn.st2 );
    base_int_t Tdims[_HADA_DDIMS], Thow[_HADA_DDIMS];
    base_int_t hdim = 0;
    getdimshowmany( &vIn, ( &Tdims[0] ), ( &Thow[0] ), &hdim );
    for ( int i = 0; i < _HADA_DDIMS; ++i ) {
        printf( "Tdims[%zu]\thow[%zu]\n", Tdims[i], Thow[i] );
    }
    printf( "hdim %zu\n", hdim );
    base_int_t Hdims[_HADA_DDIMS * hdim];
    gethdims( ( &Tdims[0] ), ( &Hdims[0] ) );
    // base_int_t *Hdims = (base_int_t*) malloc(sizeof(base_int_t) * _HADA_DDIMS * hdim);
    // gethdims( (&Tdims[0]),  Hdims);
    for ( base_int_t i = 0; i < _HADA_DDIMS; ++i ) {
        for ( base_int_t j = 0; j < hdim; ++j ) {
            printf( "%zu\t", Hdims[j + i * hdim] );
        }
        printf( "\n" );
    }
    printf( "\n" );
    for ( base_int_t i = 0; i < _HADA_DDIMS * hdim; ++i ) {
        printf( "%zu ", Hdims[i] );
    }
    printf( "\n" );

    showparsedhdims( ( &Hdims[0] ), hdim );
    // free(Hdims);Hdims=NULL;
    return ierr;
}