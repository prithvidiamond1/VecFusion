#include <stdint.h>

void vectorized_s2251(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations; nl++) {
        // i=0: s=0.0 (initial value)
        a[0] = 0.0f * e[0];
        float s = b[0] + c[0];
        b[0] = a[0] + d[0];

        // The loop has a loop-carried dependency through s:
        //   s_{i} = b[i] + c[i]  (using original b[i])
        //   a[i]  = s_{i-1} * e[i]
        //   b[i]  = a[i] + d[i]
        //
        // Because s depends on the original b[i] (before update),
        // and a[i] depends on s from the previous iteration,
        // this is a serial recurrence. We can't fully vectorize the
        // dependency chain, but we can unroll to help the compiler
        // pipeline and reduce loop overhead.

        int i = 1;
        // Unroll by 4
        for (; i <= LEN_1D - 4; i += 4) {
            float s0 = s;
            float a0 = s0 * e[i];
            float s1 = b[i] + c[i];
            b[i] = a0 + d[i];
            a[i] = a0;

            float a1 = s1 * e[i+1];
            float s2 = b[i+1] + c[i+1];
            b[i+1] = a1 + d[i+1];
            a[i+1] = a1;

            float a2 = s2 * e[i+2];
            float s3 = b[i+2] + c[i+2];
            b[i+2] = a2 + d[i+2];
            a[i+2] = a2;

            float a3 = s3 * e[i+3];
            float s4 = b[i+3] + c[i+3];
            b[i+3] = a3 + d[i+3];
            a[i+3] = a3;

            s = s4;
        }

        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            a[i] = s * e[i];
            s = b[i] + c[i];
            b[i] = a[i] + d[i];
        }
    }
}
