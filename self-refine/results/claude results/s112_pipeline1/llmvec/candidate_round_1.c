#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s112(float *a, float *b, int iterations, int LEN_1D) {
    for (int nl = 0; nl < 3 * iterations; nl++) {
        for (int i = LEN_1D - 2; i >= 0; i--) {
            a[i + 1] = a[i] + b[i];
        }
    }
}

        void vectorized_s112(float *a, float *b, int iterations, int LEN_1D) {
    for (int nl = 0; nl < 3 * iterations; nl++) {
        // This loop has a loop-carried dependency: a[i+1] depends on a[i] from previous iteration
        // (iterating backwards: i from LEN_1D-2 down to 0, writing a[i+1] = a[i] + b[i])
        // Each iteration writes a[i+1] and reads a[i], but since we go downward,
        // a[i] was NOT written by a previous iteration in this sweep (we write higher indices first).
        // So a[i] is always the original a[i] from before the inner loop started.
        // This means we can vectorize by preloading a[] snapshot.

        // We need a temporary copy of a[] to read from, since writes don't affect reads
        // (each a[i] read is the value before this inner loop pass).
        // Actually: i goes from LEN_1D-2 down to 0.
        // a[LEN_1D-1] = a[LEN_1D-2] + b[LEN_1D-2]
        // a[LEN_1D-2] = a[LEN_1D-3] + b[LEN_1D-3]
        // ...
        // a[1] = a[0] + b[0]
        // Each read of a[i] uses the ORIGINAL a[i] (not yet overwritten in this pass,
        // since we write a[i+1] which is a higher index, already processed).
        // So reads are always from original values. We can vectorize directly.

        int i = LEN_1D - 2;

        // Process 8 elements at a time (unrolled)
        for (; i >= 7; i -= 8) {
            float a0 = a[i]   + b[i];
            float a1 = a[i-1] + b[i-1];
            float a2 = a[i-2] + b[i-2];
            float a3 = a[i-3] + b[i-3];
            float a4 = a[i-4] + b[i-4];
            float a5 = a[i-5] + b[i-5];
            float a6 = a[i-6] + b[i-6];
            float a7 = a[i-7] + b[i-7];
            a[i+1]   = a0;
            a[i]     = a1;
            a[i-1]   = a2;
            a[i-2]   = a3;
            a[i-3]   = a4;
            a[i-4]   = a5;
            a[i-5]   = a6;
            a[i-6]   = a7;
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
            const int n = 128;
            uint32_t seed = 7u;
            float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; int iterations = 5; int LEN_1D = n;

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, n, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar));
                s112(a_scalar, b_scalar, iterations, LEN_1D); vectorized_s112(a_vector, b_vector, iterations, LEN_1D);
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
