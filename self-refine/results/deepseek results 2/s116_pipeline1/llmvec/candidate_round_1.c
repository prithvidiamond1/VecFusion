#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s116(int iterations, int LEN_1D, float* a)
{
    for (int nl = 0; nl < iterations*10; nl++) {
        for (int i = 0; i < LEN_1D - 5; i += 5) {
            a[i] = a[i + 1] * a[i];
            a[i + 1] = a[i + 2] * a[i + 1];
            a[i + 2] = a[i + 3] * a[i + 2];
            a[i + 3] = a[i + 4] * a[i + 3];
            a[i + 4] = a[i + 5] * a[i + 4];
        }
    }
}

        void vectorized_s116(int iterations, int LEN_1D, float* a)
{
    for (int nl = 0; nl < iterations * 10; nl++) {
        // The inner loop has loop-carried dependencies within each group of 5,
        // but each iteration of i is independent of the next (i += 5).
        // We can unroll/vectorize across multiple groups of 5 at once.

        int limit = LEN_1D - 5;
        int i = 0;

        // Process 4 groups of 5 at a time (20 elements per iteration)
        for (; i + 20 <= limit; i += 20) {
            // Group 0: i+0..i+4
            float a0_0 = a[i + 0];
            float a0_1 = a[i + 1];
            float a0_2 = a[i + 2];
            float a0_3 = a[i + 3];
            float a0_4 = a[i + 4];
            float a0_5 = a[i + 5];

            // Group 1: i+5..i+9
            float a1_0 = a[i + 5];
            float a1_1 = a[i + 6];
            float a1_2 = a[i + 7];
            float a1_3 = a[i + 8];
            float a1_4 = a[i + 9];
            float a1_5 = a[i + 10];

            // Group 2: i+10..i+14
            float a2_0 = a[i + 10];
            float a2_1 = a[i + 11];
            float a2_2 = a[i + 12];
            float a2_3 = a[i + 13];
            float a2_4 = a[i + 14];
            float a2_5 = a[i + 15];

            // Group 3: i+15..i+19
            float a3_0 = a[i + 15];
            float a3_1 = a[i + 16];
            float a3_2 = a[i + 17];
            float a3_3 = a[i + 18];
            float a3_4 = a[i + 19];
            float a3_5 = a[i + 20];

            // Compute group 0
            a[i + 0] = a0_1 * a0_0;
            a[i + 1] = a0_2 * a0_1;
            a[i + 2] = a0_3 * a0_2;
            a[i + 3] = a0_4 * a0_3;
            a[i + 4] = a0_5 * a0_4;

            // Compute group 1
            a[i + 5] = a1_1 * a1_0;
            a[i + 6] = a1_2 * a1_1;
            a[i + 7] = a1_3 * a1_2;
            a[i + 8] = a1_4 * a1_3;
            a[i + 9] = a1_5 * a1_4;

            // Compute group 2
            a[i + 10] = a2_1 * a2_0;
            a[i + 11] = a2_2 * a2_1;
            a[i + 12] = a2_3 * a2_2;
            a[i + 13] = a2_4 * a2_3;
            a[i + 14] = a2_5 * a2_4;

            // Compute group 3
            a[i + 15] = a3_1 * a3_0;
            a[i + 16] = a3_2 * a3_1;
            a[i + 17] = a3_3 * a3_2;
            a[i + 18] = a3_4 * a3_3;
            a[i + 19] = a3_5 * a3_4;
        }

        // Scalar cleanup for remaining groups
        for (; i < limit; i += 5) {
            a[i]     = a[i + 1] * a[i];
            a[i + 1] = a[i + 2] * a[i + 1];
            a[i + 2] = a[i + 3] * a[i + 2];
            a[i + 3] = a[i + 4] * a[i + 3];
            a[i + 4] = a[i + 5] * a[i + 4];
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar));
                s116(iterations, LEN_1D, a_scalar); vectorized_s116(iterations, LEN_1D, a_vector);
                for (int i = 0; i < arr_len; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
