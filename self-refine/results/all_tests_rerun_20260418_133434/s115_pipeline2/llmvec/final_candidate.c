#include <stdint.h>

void vectorized_s115(int iterations, float* a, float aa[256][256])
{
    int loop_count = 1000 * (iterations / 256);

    typedef float v4f __attribute__((vector_size(16)));
    const int VEC_WIDTH = 4;

    for (int nl = 0; nl < loop_count; nl++) {
        for (int j = 0; j < 256; j++) {
            float aj = a[j];
            float* aa_row = aa[j];

            v4f vaj = (v4f){aj, aj, aj, aj};

            int i = j + 1;
            // Align to vector boundary
            for (; i < 256 && (i % VEC_WIDTH != 0); i++) {
                a[i] -= aa_row[i] * aj;
            }
            // Vectorized portion
            for (; i + VEC_WIDTH <= 256; i += VEC_WIDTH) {
                v4f va, vaa;
                __builtin_memcpy(&va,  a      + i, sizeof(v4f));
                __builtin_memcpy(&vaa, aa_row + i, sizeof(v4f));
                va = va - vaa * vaj;
                __builtin_memcpy(a + i, &va, sizeof(v4f));
            }
            // Scalar tail
            for (; i < 256; i++) {
                a[i] -= aa_row[i] * aj;
            }
        }
    }
}
