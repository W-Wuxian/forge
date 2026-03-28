#include "fwht_tester.h"

// ref solution https://en.wikipedia.org/wiki/Fast_Walsh%E2%80%93Hadamard_transform
#define REF_SOL_IDX_0 ( (double)4 )
#define REF_SOL_IDX_1 ( (double)2 )
#define REF_SOL_IDX_2 ( (double)0 )
#define REF_SOL_IDX_3 ( (double)-2 )
#define REF_SOL_IDX_4 ( (double)0 )
#define REF_SOL_IDX_5 ( (double)2 )
#define REF_SOL_IDX_6 ( (double)0 )
#define REF_SOL_IDX_7 ( (double)2 )

static const double REF_SOL[] = { REF_SOL_IDX_0, REF_SOL_IDX_1, REF_SOL_IDX_2, REF_SOL_IDX_3,
                                  REF_SOL_IDX_4, REF_SOL_IDX_5, REF_SOL_IDX_6, REF_SOL_IDX_7 };

base_int_t
test_base_dummy_fwht()
{
    base_int_t ierr        = 0;
    base_int_t j           = 0;
    base_int_t ref_sol_dim = 8 * 1;  // M = 8 N = 1

    double data[ref_sol_dim];
    for ( base_int_t i = 0; i < ref_sol_dim; i++ ) {
        data[i] = (double)0;
    }
    data[0] = 1.;
    data[1] = 0.;
    data[2] = 1.;
    data[3] = 0.;
    data[4] = 0.;
    data[5] = 1.;
    data[6] = 1.;
    data[7] = 0.;
    base_dummy_fwht( &data[0], ref_sol_dim );

    for ( j = 0; j < ref_sol_dim; ++j ) {
        if ( data[j] != REF_SOL[j] ) {
            PRINT_INFO();
            printf( "data[%zu] = %f != %f\n", j, data[j], REF_SOL[j] );
            ierr += 1;
        }
    }

    return ierr;
}