#include <stddef.h>

void vectorized_s221(int iterations, int LEN_1D, float *a, float *b, float *c, float *d) {
    int total_iters = iterations;
    for (int nl = 0; nl < total_iters; nl++) {
        // Scalar loop due to loop-carried dependency on b[i-1]
        for (int i = 1; i < LEN_1D; i++) {
            a[i] += c[i] * d[i];
            b[i] = b[i - 1] + a[i] + d[i];
        }
    }
}
