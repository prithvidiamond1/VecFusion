#include <string.h>

void vectorized_s293(int iterations, int LEN_1D, float* a) {
    for (int nl = 0; nl < 4 * iterations; nl++) {
        float val = a[0];

        typedef float float4 __attribute__((ext_vector_type(4)));
        typedef float float8 __attribute__((ext_vector_type(8)));

        float8 vval;
        vval[0] = val; vval[1] = val; vval[2] = val; vval[3] = val;
        vval[4] = val; vval[5] = val; vval[6] = val; vval[7] = val;

        int i = 0;
        int limit = LEN_1D - (LEN_1D % 8);
        for (; i < limit; i += 8) {
            *((float8*)(a + i)) = vval;
        }
        for (; i < LEN_1D; i++) {
            a[i] = val;
        }
    }
}
