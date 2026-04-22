#include <stdint.h>

void vectorized_s2251(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e) {
    for (int nl = 0; nl < iterations; nl++) {
        float s_prev = 0.0f;

        // The loop has a carried dependency through s_prev (scalar), but the
        // body can be partially vectorized: a[i] = s_prev * e[i] depends on
        // s_prev from previous iteration (scalar chain), so we keep the scalar
        // structure but unroll 4x to help the compiler vectorize independent ops.

        int i = 0;
        // Scalar loop - the recurrence s_prev = b[i] + c[i] prevents full vectorization
        // but we unroll to expose ILP
        for (; i + 3 < LEN_1D; i += 4) {
            float s0 = b[i+0] + c[i+0];
            float s1 = b[i+1] + c[i+1];
            float s2 = b[i+2] + c[i+2];
            float s3 = b[i+3] + c[i+3];

            float a0 = s_prev * e[i+0];
            float a1 = s0    * e[i+1];
            float a2 = s1    * e[i+2];
            float a3 = s2    * e[i+3];

            a[i+0] = a0;
            a[i+1] = a1;
            a[i+2] = a2;
            a[i+3] = a3;

            b[i+0] = a0 + d[i+0];
            b[i+1] = a1 + d[i+1];
            b[i+2] = a2 + d[i+2];
            b[i+3] = a3 + d[i+3];

            s_prev = s3;
        }

        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            float s_next = b[i] + c[i];
            a[i] = s_prev * e[i];
            b[i] = a[i] + d[i];
            s_prev = s_next;
        }
    }
}
