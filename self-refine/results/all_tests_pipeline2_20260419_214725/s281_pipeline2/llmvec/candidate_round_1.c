#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s281(int iterations, int LEN_1D, float* a, float* b, float* c)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Process second half first (i = LEN_1D/2 to LEN_1D-1)
        // These read from a[LEN_1D-i-1] which are in the first half (unchanged)
        for (int i = LEN_1D/2; i < LEN_1D; i++) {
            float x = a[LEN_1D-i-1] + b[i] * c[i];
            a[i] = x - (float)1.0;
            b[i] = x;
        }

        // Process first half (i = 0 to LEN_1D/2-1)
        // These read from a[LEN_1D-i-1] which are in the second half (now updated)
        for (int i = 0; i < LEN_1D/2; i++) {
            float x = a[LEN_1D-i-1] + b[i] * c[i];
            a[i] = x - (float)1.0;
            b[i] = x;
        }
    }
}

        #include <stddef.h>

void vectorized_s281(int iterations, int LEN_1D, float* a, float* b, float* c) {
    if (LEN_1D <= 0 || iterations <= 0) return;

    int half = LEN_1D / 2;
    int second_half_start = half;
    int second_half_end = LEN_1D;

    for (int nl = 0; nl < iterations; nl++) {
        // Process second half first (i = half to LEN_1D-1)
        int i = second_half_start;
        for (; i + 3 < second_half_end; i += 4) {
            // Vectorized computation for 4 elements
            float b0 = b[i];
            float b1 = b[i+1];
            float b2 = b[i+2];
            float b3 = b[i+3];

            float c0 = c[i];
            float c1 = c[i+1];
            float c2 = c[i+2];
            float c3 = c[i+3];

            int idx0 = LEN_1D - i - 1;
            int idx1 = LEN_1D - (i+1) - 1;
            int idx2 = LEN_1D - (i+2) - 1;
            int idx3 = LEN_1D - (i+3) - 1;

            float a_src0 = a[idx0];
            float a_src1 = a[idx1];
            float a_src2 = a[idx2];
            float a_src3 = a[idx3];

            float x0 = a_src0 + b0 * c0;
            float x1 = a_src1 + b1 * c1;
            float x2 = a_src2 + b2 * c2;
            float x3 = a_src3 + b3 * c3;

            a[i] = x0 - 1.0f;
            a[i+1] = x1 - 1.0f;
            a[i+2] = x2 - 1.0f;
            a[i+3] = x3 - 1.0f;

            b[i] = x0;
            b[i+1] = x1;
            b[i+2] = x2;
            b[i+3] = x3;
        }
        // Scalar tail for second half
        for (; i < second_half_end; i++) {
            float x = a[LEN_1D - i - 1] + b[i] * c[i];
            a[i] = x - 1.0f;
            b[i] = x;
        }

        // Process first half (i = 0 to half-1)
        i = 0;
        for (; i + 3 < half; i += 4) {
            // Vectorized computation for 4 elements
            float b0 = b[i];
            float b1 = b[i+1];
            float b2 = b[i+2];
            float b3 = b[i+3];

            float c0 = c[i];
            float c1 = c[i+1];
            float c2 = c[i+2];
            float c3 = c[i+3];

            int idx0 = LEN_1D - i - 1;
            int idx1 = LEN_1D - (i+1) - 1;
            int idx2 = LEN_1D - (i+2) - 1;
            int idx3 = LEN_1D - (i+3) - 1;

            float a_src0 = a[idx0];
            float a_src1 = a[idx1];
            float a_src2 = a[idx2];
            float a_src3 = a[idx3];

            float x0 = a_src0 + b0 * c0;
            float x1 = a_src1 + b1 * c1;
            float x2 = a_src2 + b2 * c2;
            float x3 = a_src3 + b3 * c3;

            a[i] = x0 - 1.0f;
            a[i+1] = x1 - 1.0f;
            a[i+2] = x2 - 1.0f;
            a[i+3] = x3 - 1.0f;

            b[i] = x0;
            b[i+1] = x1;
            b[i+2] = x2;
            b[i+3] = x3;
        }
        // Scalar tail for first half
        for (; i < half; i++) {
            float x = a[LEN_1D - i - 1] + b[i] * c[i];
            a[i] = x - 1.0f;
            b[i] = x;
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
            const int arr_len = 128;
            uint32_t seed = 7u;
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar));
                s281(iterations, LEN_1D, a_scalar, b_scalar, c_scalar); vectorized_s281(iterations, LEN_1D, a_vector, b_vector, c_vector);
                for (int i = 0; i < arr_len; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((b_scalar[i]) - (b_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter b on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((c_scalar[i]) - (c_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter c on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
