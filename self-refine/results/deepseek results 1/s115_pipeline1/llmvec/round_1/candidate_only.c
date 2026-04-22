#include <stdint.h>

void vectorized_s115(int iterations, float* a, float aa[256][256])
{
    for (int nl = 0; nl < 1000*(iterations/256); nl++) {
        for (int j = 0; j < 256; j++) {
            float aj = a[j];
            float* aa_j = aa[j];
            int i = j + 1;
            // Vector-friendly unrolled loop (groups of 8)
            for (; i <= 256 - 8; i += 8) {
                a[i+0] -= aa_j[i+0] * aj;
                a[i+1] -= aa_j[i+1] * aj;
                a[i+2] -= aa_j[i+2] * aj;
                a[i+3] -= aa_j[i+3] * aj;
                a[i+4] -= aa_j[i+4] * aj;
                a[i+5] -= aa_j[i+5] * aj;
                a[i+6] -= aa_j[i+6] * aj;
                a[i+7] -= aa_j[i+7] * aj;
            }
            // Scalar cleanup tail
            for (; i < 256; i++) {
                a[i] -= aa_j[i] * aj;
            }
        }
    }
}
