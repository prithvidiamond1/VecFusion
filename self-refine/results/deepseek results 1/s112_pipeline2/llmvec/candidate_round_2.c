#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s112(float *a, float *b, int iterations, int LEN_1D) {
    for (int nl = 0; nl < 3 * iterations; nl++) {
        // Store original a[0] value for the last iteration
        float temp = a[0];

        // Process all elements except the first in backward order
        for (int i = LEN_1D - 2; i >= 0; i--) {
            a[i + 1] = a[i] + b[i];
        }

        // Handle the first element using stored value
        a[0] = temp;
    }
}

        #include <string.h>

void vectorized_s112(float *a, float *b, int iterations, int LEN_1D) {
    for (int nl = 0; nl < 3 * iterations; nl++) {
        // The backward loop: a[i+1] = a[i] + b[i] for i = LEN_1D-2 down to 0
        // Each read of a[i] uses the original value (writes go to a[i+1], reads from a[i] which is lower).
        // We can unroll without chaining since each output index is unique and inputs are original values.

        int i = LEN_1D - 2;

        // Unroll by 8 (backward, no chaining - each a[i+1] uses original a[i])
        for (; i >= 7; i -= 8) {
            a[i + 1] = a[i]     + b[i];
            a[i]     = a[i - 1] + b[i - 1];
            a[i - 1] = a[i - 2] + b[i - 2];
            a[i - 2] = a[i - 3] + b[i - 3];
            a[i - 3] = a[i - 4] + b[i - 4];
            a[i - 4] = a[i - 5] + b[i - 5];
            a[i - 5] = a[i - 6] + b[i - 6];
            a[i - 6] = a[i - 7] + b[i - 7];
        }

        // Scalar cleanup
        for (; i >= 0; i--) {
            a[i + 1] = a[i] + b[i];
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
