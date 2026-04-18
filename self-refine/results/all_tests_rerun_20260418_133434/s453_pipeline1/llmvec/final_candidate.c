#include <stdint.h>

void vectorized_s453(int iterations, int LEN_1D, float* a, float* b)
{
    float s;
    for (int nl = 0; nl < iterations * 2; nl++) {
        s = 0.0f;

        // Precompute s for each index: s at index i = 2.0f * (i+1)
        // a[i] = s * b[i] where s = 2*(i+1)
        // We can vectorize by precomputing the multiplier array inline

        int i = 0;
        float s_val = 0.0f;

        // Unroll by 8
        for (; i <= LEN_1D - 8; i += 8) {
            float s0 = s_val + 2.0f;
            float s1 = s_val + 4.0f;
            float s2 = s_val + 6.0f;
            float s3 = s_val + 8.0f;
            float s4 = s_val + 10.0f;
            float s5 = s_val + 12.0f;
            float s6 = s_val + 14.0f;
            float s7 = s_val + 16.0f;

            a[i+0] = s0 * b[i+0];
            a[i+1] = s1 * b[i+1];
            a[i+2] = s2 * b[i+2];
            a[i+3] = s3 * b[i+3];
            a[i+4] = s4 * b[i+4];
            a[i+5] = s5 * b[i+5];
            a[i+6] = s6 * b[i+6];
            a[i+7] = s7 * b[i+7];

            s_val += 16.0f;
        }

        // Scalar tail
        for (; i < LEN_1D; i++) {
            s_val += 2.0f;
            a[i] = s_val * b[i];
        }

        (void)s;
    }
}
