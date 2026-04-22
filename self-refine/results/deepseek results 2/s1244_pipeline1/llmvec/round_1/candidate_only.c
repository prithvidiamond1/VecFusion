#include <stdint.h>

void vectorized_s1244(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        int i = 0;
        int limit = LEN_1D - 1;

#if defined(__clang__) || defined(__GNUC__)
        typedef float float4 __attribute__((vector_size(16)));
        int vec_limit = limit - (limit % 4);

        for (; i < vec_limit; i += 4) {
            float4 bv, cv, av;
            __builtin_memcpy(&bv, b + i, 16);
            __builtin_memcpy(&cv, c + i, 16);
            av = bv + cv * cv + bv * bv + cv;
            __builtin_memcpy(a + i, &av, 16);
        }

        /* scalar tail for a[] */
        for (int j = i; j < limit; j++) {
            a[j] = b[j] + c[j] * c[j] + b[j] * b[j] + c[j];
        }

        /* Now compute d[i] = a[i] + a[i+1] using vectors */
        i = 0;
        /* We need a[i] and a[i+1], so we can vectorize d as well */
        int vec_limit2 = limit - (limit % 4);
        for (; i < vec_limit2; i += 4) {
            float4 av0, av1, dv;
            __builtin_memcpy(&av0, a + i, 16);
            __builtin_memcpy(&av1, a + i + 1, 16);
            dv = av0 + av1;
            __builtin_memcpy(d + i, &dv, 16);
        }
        for (int j = i; j < limit; j++) {
            d[j] = a[j] + a[j + 1];
        }
#else
        for (; i < limit; i++) {
            a[i] = b[i] + c[i] * c[i] + b[i] * b[i] + c[i];
            d[i] = a[i] + a[i + 1];
        }
#endif
    }
}
