#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s342(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < iterations; nl++) {
        int masks[LEN_1D];
        int prefix[LEN_1D];

        // First pass: compute masks (vectorizable)
        for (int i = 0; i < LEN_1D; i++) {
            masks[i] = (a[i] > (float)0.) ? 1 : 0;
        }

        // Second pass: prefix sum - sequential, isolated dependency
        prefix[0] = masks[0] - 1;
        for (int i = 1; i < LEN_1D; i++) {
            prefix[i] = prefix[i-1] + masks[i];
        }

        // Third pass: conditional update using precomputed indices (vectorizable gather+mask)
        for (int i = 0; i < LEN_1D; i++) {
            float bval = b[prefix[i] < 0 ? 0 : prefix[i]];
            a[i] = masks[i] * bval + (1 - masks[i]) * a[i];
        }
    }
}

        #include <stdint.h>

void vectorized_s342(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < iterations; nl++) {
        int masks[LEN_1D];
        int prefix[LEN_1D];

        // First pass: compute masks (unrolled 4x)
        int i = 0;
        for (; i <= LEN_1D - 4; i += 4) {
            masks[i+0] = (a[i+0] > 0.f) ? 1 : 0;
            masks[i+1] = (a[i+1] > 0.f) ? 1 : 0;
            masks[i+2] = (a[i+2] > 0.f) ? 1 : 0;
            masks[i+3] = (a[i+3] > 0.f) ? 1 : 0;
        }
        for (; i < LEN_1D; i++) {
            masks[i] = (a[i] > 0.f) ? 1 : 0;
        }

        // Second pass: prefix sum - sequential dependency, cannot vectorize
        prefix[0] = masks[0] - 1;
        for (i = 1; i < LEN_1D; i++) {
            prefix[i] = prefix[i-1] + masks[i];
        }

        // Third pass: conditional update using precomputed indices (unrolled 4x)
        i = 0;
        for (; i <= LEN_1D - 4; i += 4) {
            int idx0 = prefix[i+0] < 0 ? 0 : prefix[i+0];
            int idx1 = prefix[i+1] < 0 ? 0 : prefix[i+1];
            int idx2 = prefix[i+2] < 0 ? 0 : prefix[i+2];
            int idx3 = prefix[i+3] < 0 ? 0 : prefix[i+3];

            float bval0 = b[idx0];
            float bval1 = b[idx1];
            float bval2 = b[idx2];
            float bval3 = b[idx3];

            int m0 = masks[i+0];
            int m1 = masks[i+1];
            int m2 = masks[i+2];
            int m3 = masks[i+3];

            a[i+0] = m0 * bval0 + (1 - m0) * a[i+0];
            a[i+1] = m1 * bval1 + (1 - m1) * a[i+1];
            a[i+2] = m2 * bval2 + (1 - m2) * a[i+2];
            a[i+3] = m3 * bval3 + (1 - m3) * a[i+3];
        }
        for (; i < LEN_1D; i++) {
            int idx = prefix[i] < 0 ? 0 : prefix[i];
            float bval = b[idx];
            int m = masks[i];
            a[i] = m * bval + (1 - m) * a[i];
        }
    }
}

        static uint32_t next_u32(uint32_t *state) {
            *state = (*state * 1664525u) + 1013904223u;
            return *state;
        }

        static void fill_i32(int *buf, int n, uint32_t *state) {
            for (int i = 0; i < n; ++i) {
                buf[i] = (int)(next_u32(state) % 2001u) - 1000;
            }
        }

        static void fill_f32(float *buf, int n, uint32_t *state) {
            for (int i = 0; i < n; ++i) {
                buf[i] = ((float)(next_u32(state) % 2001u) - 1000.0f) / 17.0f;
            }
        }

        static void fill_f64(double *buf, int n, uint32_t *state) {
            for (int i = 0; i < n; ++i) {
                buf[i] = ((double)(next_u32(state) % 2001u) - 1000.0) / 17.0;
            }
        }

        int main(void) {
            const int n = 128;
            uint32_t seed = 7u;
            int iterations = 5; int LEN_1D = n; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, n, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar));
                s342(iterations, LEN_1D, a_scalar, b_scalar); vectorized_s342(iterations, LEN_1D, a_vector, b_vector);
                for (int i = 0; i < n; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((b_scalar[i]) - (b_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter b on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
