#include <stdint.h>

void vectorized_s1113(int iterations, int LEN_1D, float* a, float* b)
{
    typedef float float8 __attribute__((vector_size(32)));
    for (int nl = 0; nl < 2*iterations; nl++) {
        float scalar_val = a[LEN_1D/2];
        float8 vscalar = {scalar_val, scalar_val, scalar_val, scalar_val,
                          scalar_val, scalar_val, scalar_val, scalar_val};
        int i = 0;
        for (; i <= LEN_1D - 8; i += 8) {
            float8 vb;
            __builtin_memcpy(&vb, b + i, 32);
            float8 va = vscalar + vb;
            __builtin_memcpy(a + i, &va, 32);
        }
        for (; i < LEN_1D; i++) {
            a[i] = scalar_val + b[i];
        }
    }
}
