#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s221(int iterations, int LEN_1D, float*a,float*b,float*c,float*d)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        // Store original b[0] value for prefix sum initialization
        float b0_original = b[0];

        // First compute all a[i] updates (fully independent)
        for (int i = 1; i < LEN_1D; i++) {
            a[i] += c[i] * d[i];
        }

        // Compute prefix sum for b[i] using original b[0]
        // This preserves the exact recurrence pattern b[i] = b[i-1] + a[i] + d[i]
        b[1] = b0_original + a[1] + d[1];
        for (int i = 2; i < LEN_1D; i++) {
            b[i] = b[i-1] + a[i] + d[i];
        }
    }
}

        #include <stddef.h>

void vectorized_s221(int iterations, int LEN_1D, float* a, float* b, float* c, float* d) {
    int loop_count = iterations / 2;

    for (int nl = 0; nl < loop_count; nl++) {
        // Store original b[0] value for prefix sum initialization
        float b0_original = b[0];

        // Vectorize the independent a[i] updates
        int i = 1;
        // Process in chunks of 4 for better vectorization potential
        for (; i + 3 < LEN_1D; i += 4) {
            a[i] += c[i] * d[i];
            a[i + 1] += c[i + 1] * d[i + 1];
            a[i + 2] += c[i + 2] * d[i + 2];
            a[i + 3] += c[i + 3] * d[i + 3];
        }
        // Scalar cleanup for remaining elements
        for (; i < LEN_1D; i++) {
            a[i] += c[i] * d[i];
        }

        // Compute prefix sum for b[i] - recurrence prevents full vectorization
        // but we can unroll to improve instruction-level parallelism
        b[1] = b0_original + a[1] + d[1];

        i = 2;
        // Unrolled prefix sum computation
        for (; i + 3 < LEN_1D; i += 4) {
            b[i] = b[i - 1] + a[i] + d[i];
            b[i + 1] = b[i] + a[i + 1] + d[i + 1];
            b[i + 2] = b[i + 1] + a[i + 2] + d[i + 2];
            b[i + 3] = b[i + 2] + a[i + 3] + d[i + 3];
        }
        // Scalar cleanup for remaining elements
        for (; i < LEN_1D; i++) {
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
            const int arr_len = 128;
            uint32_t seed = 7u;
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, arr_len, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar));
                s221(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar); vectorized_s221(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector);
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
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((d_scalar[i]) - (d_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter d on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
