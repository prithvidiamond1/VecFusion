#include <stdint.h>

void vectorized_s256(int iterations, float* a, float* d, float aa[256][256], float bb[256][256])
{
    for (int nl = 0; nl < 10*(iterations/256); nl++) {
        for (int i = 0; i < 256; i++) {
            // j=1: a[1] = 1.0f - a[0]
            // j=2: a[2] = 1.0f - a[1]
            // Each iteration depends on the previous a[j-1], so this is a
            // recurrence. We cannot vectorize the a[j] computation directly,
            // but we can observe the pattern:
            // a[1] = 1.0f - a[0]
            // a[2] = 1.0f - a[1] = 1.0f - (1.0f - a[0]) = a[0]
            // a[3] = 1.0f - a[2] = 1.0f - a[0]
            // a[4] = 1.0f - a[3] = a[0]
            // So: a[j] = (j is odd) ? (1.0f - a[0]) : a[0]  for j >= 1
            // We can use this to vectorize the aa computation.

            float a0 = a[0];
            float a_odd  = 1.0f - a0;  // a[j] when j is odd
            float a_even = a0;          // a[j] when j is even (j>=2)

            // Fill a[] array first (scalar, but simple)
            for (int j = 1; j < 256; j++) {
                a[j] = (j & 1) ? a_odd : a_even;
            }

            // Now vectorize the aa computation using unrolling
            // aa[j][i] = a[j] + bb[j][i] * d[j]
            // Unroll by 8
            int j = 1;
            for (; j <= 255 - 7; j += 8) {
                aa[j+0][i] = a[j+0] + bb[j+0][i] * d[j+0];
                aa[j+1][i] = a[j+1] + bb[j+1][i] * d[j+1];
                aa[j+2][i] = a[j+2] + bb[j+2][i] * d[j+2];
                aa[j+3][i] = a[j+3] + bb[j+3][i] * d[j+3];
                aa[j+4][i] = a[j+4] + bb[j+4][i] * d[j+4];
                aa[j+5][i] = a[j+5] + bb[j+5][i] * d[j+5];
                aa[j+6][i] = a[j+6] + bb[j+6][i] * d[j+6];
                aa[j+7][i] = a[j+7] + bb[j+7][i] * d[j+7];
            }
            for (; j < 256; j++) {
                aa[j][i] = a[j] + bb[j][i] * d[j];
            }
        }
    }
}
