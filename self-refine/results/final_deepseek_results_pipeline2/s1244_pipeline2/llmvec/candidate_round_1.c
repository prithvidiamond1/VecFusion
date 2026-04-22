#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s1244(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Store original a[i+1] values before modifying a[i]
        for (int i = 0; i < LEN_1D-1; i++) {
            float a_next = a[i+1];  // Store original a[i+1]
            a[i] = b[i] + c[i] * c[i] + b[i]*b[i] + c[i];
            d[i] = a[i] + a_next;   // Use original a[i+1]
        }
    }
}

        #include <stddef.h>

void vectorized_s1244(int iterations, int LEN_1D, float* a, float* b, float* c, float* d) {
    if (LEN_1D <= 1) return;

    int n = LEN_1D - 1;

    for (int nl = 0; nl < iterations; nl++) {
        // Vectorized main loop
        int i = 0;
        for (; i + 3 < n; i += 4) {
            // Load original a[i+1] values
            float a_next0 = a[i+1];
            float a_next1 = a[i+2];
            float a_next2 = a[i+3];
            float a_next3 = a[i+4];

            // Compute a[i] values
            float b0 = b[i];
            float b1 = b[i+1];
            float b2 = b[i+2];
            float b3 = b[i+3];

            float c0 = c[i];
            float c1 = c[i+1];
            float c2 = c[i+2];
            float c3 = c[i+3];

            float a_val0 = b0 + c0 * c0 + b0 * b0 + c0;
            float a_val1 = b1 + c1 * c1 + b1 * b1 + c1;
            float a_val2 = b2 + c2 * c2 + b2 * b2 + c2;
            float a_val3 = b3 + c3 * c3 + b3 * b3 + c3;

            // Store a[i] values
            a[i] = a_val0;
            a[i+1] = a_val1;
            a[i+2] = a_val2;
            a[i+3] = a_val3;

            // Compute d[i] using original a[i+1] values
            d[i] = a_val0 + a_next0;
            d[i+1] = a_val1 + a_next1;
            d[i+2] = a_val2 + a_next2;
            d[i+3] = a_val3 + a_next3;
        }

        // Scalar cleanup for remaining elements
        for (; i < n; i++) {
            float a_next = a[i+1];
            a[i] = b[i] + c[i] * c[i] + b[i] * b[i] + c[i];
            d[i] = a[i] + a_next;
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
                s1244(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar); vectorized_s1244(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector);
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
