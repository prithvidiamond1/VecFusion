#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s112(float *a, float *b, int iterations, int LEN_1D) {
    int total_outer = 3 * iterations;

    for (int nl = 0; nl < total_outer; nl++) {
        // Process in reverse order - no loop-carried dependency
        for (int i = LEN_1D - 2; i >= 0; i--) {
            float temp = a[i] + b[i];
            a[i + 1] = temp;
        }
    }
}

        #include <stddef.h>

void vectorized_s112(float *a, float *b, int iterations, int LEN_1D) {
    int total_outer = 3 * iterations;

    for (int nl = 0; nl < total_outer; nl++) {
        // Vectorized reverse processing
        int i = LEN_1D - 2;

        // Process vector chunks in reverse order
        for (; i >= 3; i -= 4) {
            // Load current elements
            float a0 = a[i];
            float a1 = a[i-1];
            float a2 = a[i-2];
            float a3 = a[i-3];

            float b0 = b[i];
            float b1 = b[i-1];
            float b2 = b[i-2];
            float b3 = b[i-3];

            // Compute temp values
            float temp0 = a0 + b0;
            float temp1 = a1 + b1;
            float temp2 = a2 + b2;
            float temp3 = a3 + b3;

            // Store to next positions (i+1, i, i-1, i-2)
            a[i+1] = temp0;
            a[i] = temp1;
            a[i-1] = temp2;
            a[i-2] = temp3;
        }

        // Scalar cleanup for remaining elements
        for (; i >= 0; i--) {
            float temp = a[i] + b[i];
            a[i + 1] = temp;
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
            float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; int iterations = 5; int LEN_1D = arr_len;

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar));
                s112(a_scalar, b_scalar, iterations, LEN_1D); vectorized_s112(a_vector, b_vector, iterations, LEN_1D);
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
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
