#include <string.h>

void vectorized_s293(int iterations, int LEN_1D, float* a)
{
    float val = a[0];

    typedef float float4 __attribute__((vector_size(16)));

    float4 vval;
    vval[0] = val;
    vval[1] = val;
    vval[2] = val;
    vval[3] = val;

    int i = 0;
    int limit = LEN_1D - (LEN_1D % 4);

    for (; i < limit; i += 4) {
        __builtin_memcpy(a + i, &vval, 16);
    }

    for (; i < LEN_1D; i++) {
        a[i] = val;
    }
}
