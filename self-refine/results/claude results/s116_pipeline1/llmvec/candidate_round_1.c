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

        #include <stdint.h>

void vectorized_s116(int iterations, int LEN_1D, float* a)
{
    for (int nl = 0; nl < iterations * 10; nl++) {
        // The inner loop has stride-5 updates with data dependencies between
        // consecutive iterations (a[i+5] is read, then used as a[i] in next iter).
        // Each group of 5 is independent within itself, so we can unroll manually.
        // We process multiple groups of 5 at once using manual unrolling.

        int limit = LEN_1D - 5;
        int i = 0;

        // Unroll by 4 groups of 5 (20 elements at a time)
        for (; i + 20 <= limit; i += 20) {
            // Group 0: i+0..i+4
            float a0  = a[i + 0];
            float a1  = a[i + 1];
            float a2  = a[i + 2];
            float a3  = a[i + 3];
            float a4  = a[i + 4];
            float a5  = a[i + 5];

            // Group 1: i+5..i+9
            float a6  = a[i + 6];
            float a7  = a[i + 7];
            float a8  = a[i + 8];
            float a9  = a[i + 9];
            float a10 = a[i + 10];

            // Group 2: i+10..i+14
            float a11 = a[i + 11];
            float a12 = a[i + 12];
            float a13 = a[i + 13];
            float a14 = a[i + 14];
            float a15 = a[i + 15];

            // Group 3: i+15..i+19
            float a16 = a[i + 16];
            float a17 = a[i + 17];
            float a18 = a[i + 18];
            float a19 = a[i + 19];
            float a20 = a[i + 20];

            // Compute group 0
            a[i + 0] = a1 * a0;
            a[i + 1] = a2 * a1;
            a[i + 2] = a3 * a2;
            a[i + 3] = a4 * a3;
            a[i + 4] = a5 * a4;

            // Compute group 1
            a[i + 5] = a6  * a5;
            a[i + 6] = a7  * a6;
            a[i + 7] = a8  * a7;
            a[i + 8] = a9  * a8;
            a[i + 9] = a10 * a9;

            // Compute group 2
            a[i + 10] = a11 * a10;
            a[i + 11] = a12 * a11;
            a[i + 12] = a13 * a12;
            a[i + 13] = a14 * a13;
            a[i + 14] = a15 * a14;

            // Compute group 3
            a[i + 15] = a16 * a15;
            a[i + 16] = a17 * a16;
            a[i + 17] = a18 * a17;
            a[i + 18] = a19 * a18;
            a[i + 19] = a20 * a19;
        }

        // Scalar cleanup for remaining groups of 5
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
            const int n = 128;
            uint32_t seed = 7u;
            int iterations = 5; int LEN_1D = n; float a_scalar[128]; float a_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar));
                s116(iterations, LEN_1D, a_scalar); vectorized_s116(iterations, LEN_1D, a_vector);
                for (int i = 0; i < n; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
