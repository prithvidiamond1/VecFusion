#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s221(int iterations, int LEN_1D, float*a, float*b, float*c, float*d)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        for (int i = 1; i < LEN_1D; i++) {
            a[i] += c[i] * d[i];
        }
        for (int i = 1; i < LEN_1D; i++) {
            b[i] = b[i - 1] + a[i] + d[i];
        }
    }
}

        #include <stdint.h>

void vectorized_s221(int iterations, int LEN_1D, float*a, float*b, float*c, float*d)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        // First loop: a[i] += c[i] * d[i] for i in [1, LEN_1D)
        // This loop has no dependencies, can be vectorized
        int i = 1;
        int len = LEN_1D - 1; // number of elements to process

        // Unroll by 8 for SIMD-friendly code
        int limit = 1 + (len / 8) * 8;
        for (; i < limit; i += 8) {
            a[i+0] += c[i+0] * d[i+0];
            a[i+1] += c[i+1] * d[i+1];
            a[i+2] += c[i+2] * d[i+2];
            a[i+3] += c[i+3] * d[i+3];
            a[i+4] += c[i+4] * d[i+4];
            a[i+5] += c[i+5] * d[i+5];
            a[i+6] += c[i+6] * d[i+6];
            a[i+7] += c[i+7] * d[i+7];
        }
        // Scalar cleanup
        for (; i < LEN_1D; i++) {
            a[i] += c[i] * d[i];
        }

        // Second loop: b[i] = b[i-1] + a[i] + d[i] for i in [1, LEN_1D)
        // This loop has a loop-carried dependency on b[i-1], cannot be vectorized
        for (i = 1; i < LEN_1D; i++) {
            b[i] = b[i - 1] + a[i] + d[i];
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
            int iterations = 5; int LEN_1D = n; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, n, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, n, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, n, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar));
                s221(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar); vectorized_s221(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector);
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
} for (int i = 0; i < n; ++i) {
    if (fabsf((c_scalar[i]) - (c_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter c on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((d_scalar[i]) - (d_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter d on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
