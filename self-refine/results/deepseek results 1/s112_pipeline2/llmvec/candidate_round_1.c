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
        float temp = a[0];

        // The loop has a loop-carried dependency (a[i+1] depends on a[i] from previous iteration
        // but here each a[i+1] = a[i] + b[i] where a[i] is the ORIGINAL a[i], not updated).
        // Actually looking carefully: we go backward i = LEN_1D-2 down to 0
        // a[LEN_1D-1] = a[LEN_1D-2] + b[LEN_1D-2]
        // a[LEN_1D-2] = a[LEN_1D-3] + b[LEN_1D-3]
        // ...
        // a[1] = a[0] + b[0]
        // Since we go backward, each write to a[i+1] doesn't affect a[i] which is read next.
        // So there's NO loop-carried dependency! We can vectorize.

        // Unroll by 8 for vectorization
        int i = LEN_1D - 2;

        // We need to process i from LEN_1D-2 down to 0
        // Unroll: process chunks of 8
        // To vectorize, we process contiguous memory: a[i+1] = a[i] + b[i]
        // i ranges: LEN_1D-2, LEN_1D-3, ..., 0
        // Equivalent: for j=0 to LEN_1D-2: a[j+1] = a[j] + b[j]
        // Since backward traversal has no dependency, same as forward

        // Process in chunks of 8 (forward is fine since no dependency)
        int n = LEN_1D - 1; // number of elements to compute (indices 1..LEN_1D-1)
        int j = 0;

        // Unroll by 8
        for (; j <= n - 8; j += 8) {
            a[j + 1] = a[j + 0] + b[j + 0];
            a[j + 2] = a[j + 1] + b[j + 1];
            a[j + 3] = a[j + 2] + b[j + 2];
            a[j + 4] = a[j + 3] + b[j + 3];
            a[j + 5] = a[j + 4] + b[j + 4];
            a[j + 6] = a[j + 5] + b[j + 5];
            a[j + 7] = a[j + 6] + b[j + 6];
            a[j + 8] = a[j + 7] + b[j + 7];
        }

        // Scalar cleanup
        for (; j < n; j++) {
            a[j + 1] = a[j] + b[j];
        }

        a[0] = temp;
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
