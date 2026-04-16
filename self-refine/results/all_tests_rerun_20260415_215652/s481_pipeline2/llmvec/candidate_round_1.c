#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s481(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    // Pre-scan d once to check if early exit would ever trigger
    // Since d is never modified, we only need to scan once
    for (int i = 0; i < LEN_1D; i++) {
        if (d[i] < (float)0.) {
            // Find where the early exit happens
            // Original: process nl=0 up to i, then return
            for (int j = 0; j < i; j++) {
                a[j] += b[j] * c[j];
            }
            return;
        }
    }

    // All d[i] >= 0, no early exit will occur in any iteration
    // Now the inner loop is fully vectorizable
    for (int nl = 0; nl < iterations; nl++) {
        for (int i = 0; i < LEN_1D; i++) {
            a[i] += b[i] * c[i];
        }
    }
}

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
                s481(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar); vectorized_s481(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector);
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
