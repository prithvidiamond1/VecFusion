#include <stdint.h>

void vectorized_s115(int iterations, float* a, float aa[256][256])
{
    for (int nl = 0; nl < 1000*(iterations/256); nl++) {
        for (int j = 0; j < 256; j++) {
            float aj = a[j];
            int i = j + 1;
            int limit = 256;

            // Vectorize using GCC/Clang vector extensions
            typedef float float4 __attribute__((vector_size(16)));
            typedef float float8 __attribute__((vector_size(32)));

            float8 vaj = { aj, aj, aj, aj, aj, aj, aj, aj };

            // Process 8 elements at a time
            for (; i + 7 < limit; i += 8) {
                float8 vai, vaa;
                __builtin_memcpy(&vai, &a[i], sizeof(float8));
                __builtin_memcpy(&vaa, &aa[j][i], sizeof(float8));
                vai = vai - vaa * vaj;
                __builtin_memcpy(&a[i], &vai, sizeof(float8));
            }

            // Process 4 elements at a time
            float4 vaj4 = { aj, aj, aj, aj };
            for (; i + 3 < limit; i += 4) {
                float4 vai4, vaa4;
                __builtin_memcpy(&vai4, &a[i], sizeof(float4));
                __builtin_memcpy(&vaa4, &aa[j][i], sizeof(float4));
                vai4 = vai4 - vaa4 * vaj4;
                __builtin_memcpy(&a[i], &vai4, sizeof(float4));
            }

            // Scalar tail
            for (; i < limit; i++) {
                a[i] -= aa[j][i] * aj;
            }
        }
    }
}
