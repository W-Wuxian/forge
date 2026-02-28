#include "hada.h"

#define M 32
#define N 1

int
main()
{
    base_int_t ierr = 0;
    base_int_t n = 10;
    base_int_t count = 0;
    trz(n, &count);
    printf( "n %zu has %zu trailing zeros\n", n, count );
    view_t vIn={.m=M,.n=N,.st1=1,.st2=M};
    printf("m %zu n %zu st1 %zu st2 %zu\n",vIn.m,vIn.n,vIn.st1,vIn.st2);
    base_int_t Tdims[_DDIMS], Thow[_DDIMS];
    base_int_t hdim=0;
    getdimshowmany( &vIn, (&Tdims[0]), (&Thow[0]), &hdim );
    for(int i=0; i<_DDIMS; ++i){
        printf("Tdims[%zu]\thow[%zu]\n", Tdims[i], Thow[i]);
    }
    printf("hdim %zu\n", hdim);
    base_int_t Hdims[_DDIMS * hdim];
    gethdims( (&Tdims[0]),  (&Hdims[0]));
    // base_int_t *Hdims = (base_int_t*) malloc(sizeof(base_int_t) * _DDIMS * hdim);
    // gethdims( (&Tdims[0]),  Hdims);
    printf("%zu %zu %zu\n", _DDIMS, hdim, _DDIMS*hdim);
    printf("_DDIMS as row number\n");
    for(base_int_t i=0; i<_DDIMS; ++i){
        for(base_int_t j=0; j<hdim; ++j){
            printf("%zu\t", Hdims[j + i*hdim]);
        }
        printf( "\n" );
    }
    for (base_int_t i=0;i<_DDIMS*hdim;++i)printf("%zu ",Hdims[i]);
    printf( "\n" );
    showparsedhdims((&Hdims[0]), hdim);
    //printf("sizeof(Hdims) / sizeof(Hdims[0]) %zu\n", sizeof(Hdims) / sizeof(Hdims[0]));
    //free(Hdims);Hdims=NULL;
    return ierr;
}