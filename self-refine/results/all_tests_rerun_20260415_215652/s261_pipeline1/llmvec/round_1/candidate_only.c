#include <stddef.h>

void vectorized_s261(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        // c has a loop-carried dependency: c[i] = c[i]*d[i], and a[i] uses c[i-1] (updated)
        // We must compute c serially, but can unroll by 4 with scalar steps
        int i = 1;
        // Unroll by 4 - but c dependency is serial, so we compute c first then a
        // Actually interleave: compute c[i-1] first (already done), use it for a[i], then update c[i]
        for (; i <= LEN_1D - 4 - 1; i += 4) {
            float t0, t1, t2, t3;

            // a[i] = a[i] + b[i] + c[i-1] (c[i-1] already updated)
            t0 = a[i+0] + b[i+0] + c[i-1+0];
            // c[i] = c[i] * d[i]
            float ci0 = c[i+0] * d[i+0];

            t1 = a[i+1] + b[i+1] + ci0;
            float ci1 = c[i+1] * d[i+1];

            t2 = a[i+2] + b[i+2] + ci1;
            float ci2 = c[i+2] * d[i+2];

            t3 = a[i+3] + b[i+3] + ci2;
            float ci3 = c[i+3] * d[i+3];

            a[i+0] = t0;
            a[i+1] = t1;
            a[i+2] = t2;
            a[i+3] = t3;

            c[i+0] = ci0;
            c[i+1] = ci1;
            c[i+2] = ci2;
            c[i+3] = ci3;
        }
        // scalar tail
        for (; i < LEN_1D; ++i) {
            float t = a[i] + b[i];
            a[i] = t + c[i-1];
            t = c[i] * d[i];
            c[i] = t;
        }
    }
}
