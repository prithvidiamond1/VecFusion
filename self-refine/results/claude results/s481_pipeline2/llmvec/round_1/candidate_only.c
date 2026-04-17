#include <stdint.h>

void vectorized_s481(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    // Pre-scan d once to check if early exit would ever trigger
    for (int i = 0; i < LEN_1D; i++) {
        if (d[i] < (float)0.) {
            // Process j = 0..i-1 with vectorization
            int j = 0;
            int limit = i - (i % 8);
            for (; j < limit; j += 8) {
                a[j+0] += b[j+0] * c[j+0];
                a[j+1] += b[j+1] * c[j+1];
                a[j+2] += b[j+2] * c[j+2];
                a[j+3] += b[j+3] * c[j+3];
                a[j+4] += b[j+4] * c[j+4];
                a[j+5] += b[j+5] * c[j+5];
                a[j+6] += b[j+6] * c[j+6];
                a[j+7] += b[j+7] * c[j+7];
            }
            for (; j < i; j++) {
                a[j] += b[j] * c[j];
            }
            return;
        }
    }

    // All d[i] >= 0, no early exit will occur in any iteration
    // Now the inner loop is fully vectorizable
    for (int nl = 0; nl < iterations; nl++) {
        int i = 0;
        int limit = LEN_1D - (LEN_1D % 8);
        for (; i < limit; i += 8) {
            a[i+0] += b[i+0] * c[i+0];
            a[i+1] += b[i+1] * c[i+1];
            a[i+2] += b[i+2] * c[i+2];
            a[i+3] += b[i+3] * c[i+3];
            a[i+4] += b[i+4] * c[i+4];
            a[i+5] += b[i+5] * c[i+5];
            a[i+6] += b[i+6] * c[i+6];
            a[i+7] += b[i+7] * c[i+7];
        }
        for (; i < LEN_1D; i++) {
            a[i] += b[i] * c[i];
        }
    }
}
