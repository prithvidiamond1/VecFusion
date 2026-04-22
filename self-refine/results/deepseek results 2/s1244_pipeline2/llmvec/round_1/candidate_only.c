#include <stdint.h>

void vectorized_s1244(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    typedef float v4f __attribute__((vector_size(16)));
    const int VLEN = 4;

    for (int nl = 0; nl < iterations; nl++) {
        // Vectorized first loop: a[i] = b[i] + c[i]*c[i] + b[i]*b[i] + c[i]
        int i = 0;
        for (; i <= LEN_1D - VLEN; i += VLEN) {
            v4f bv, cv, av;
            __builtin_memcpy(&bv, b + i, sizeof(v4f));
            __builtin_memcpy(&cv, c + i, sizeof(v4f));
            av = bv + cv * cv + bv * bv + cv;
            __builtin_memcpy(a + i, &av, sizeof(v4f));
        }
        for (; i < LEN_1D; i++) {
            a[i] = b[i] + c[i] * c[i] + b[i] * b[i] + c[i];
        }

        // Vectorized second loop: d[i] = a[i] + a[i+1]
        i = 0;
        for (; i <= LEN_1D - 1 - VLEN; i += VLEN) {
            v4f a0, a1, dv;
            __builtin_memcpy(&a0, a + i,     sizeof(v4f));
            __builtin_memcpy(&a1, a + i + 1, sizeof(v4f));
            dv = a0 + a1;
            __builtin_memcpy(d + i, &dv, sizeof(v4f));
        }
        for (; i < LEN_1D - 1; i++) {
            d[i] = a[i] + a[i + 1];
        }
    }
}
