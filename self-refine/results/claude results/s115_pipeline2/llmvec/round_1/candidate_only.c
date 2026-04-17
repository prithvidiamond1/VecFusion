#include <string.h>

typedef float float4 __attribute__((vector_size(16)));

void vectorized_s115(int iterations, float* a, float aa[256][256])
{
    for (int nl = 0; nl < 1000*(iterations/256); nl++) {
        for (int j = 0; j < 256; j++) {
            float aj = a[j];
            float4 vaj = {aj, aj, aj, aj};
            int i = j + 1;
            int limit = 256;
            int vec_limit = limit - ((limit - i) % 4);

            for (; i < vec_limit; i += 4) {
                float4 vai, vaa;
                __builtin_memcpy(&vai, &a[i], sizeof(float4));
                __builtin_memcpy(&vaa, &aa[j][i], sizeof(float4));
                vai = vai - vaa * vaj;
                __builtin_memcpy(&a[i], &vai, sizeof(float4));
            }

            for (; i < limit; i++) {
                a[i] -= aa[j][i] * aj;
            }
        }
    }
}
