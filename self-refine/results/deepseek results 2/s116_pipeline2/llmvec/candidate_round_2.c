#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s116(int iterations, int LEN_1D, float* a) {
    for (int nl = 0; nl < iterations * 10; nl++) {
        for (int i = 0; i < LEN_1D - 5; i += 5) {
            float t0 = a[i];
            float t1 = a[i + 1];
            float t2 = a[i + 2];
            float t3 = a[i + 3];
            float t4 = a[i + 4];
            float t5 = a[i + 5];

            a[i] = t1 * t0;
            a[i + 1] = t2 * t1;
            a[i + 2] = t3 * t2;
            a[i + 3] = t4 * t3;
            a[i + 4] = t5 * t4;
        }
    }
}

        #include <string.h>

void vectorized_s116(int iterations, int LEN_1D, float* a) {
    for (int nl = 0; nl < iterations * 10; nl++) {
        int num_groups = (LEN_1D - 1) / 5;

        // Process 4 groups at a time (20 elements) using unrolled SIMD-friendly code
        int g = 0;
        for (; g + 3 < num_groups; g += 4) {
            int i0 = g * 5;
            int i1 = (g + 1) * 5;
            int i2 = (g + 2) * 5;
            int i3 = (g + 3) * 5;

            // Load all values first
            float t0_0 = a[i0];   float t1_0 = a[i0+1]; float t2_0 = a[i0+2]; float t3_0 = a[i0+3]; float t4_0 = a[i0+4]; float t5_0 = a[i0+5];
            float t0_1 = a[i1];   float t1_1 = a[i1+1]; float t2_1 = a[i1+2]; float t3_1 = a[i1+3]; float t4_1 = a[i1+4]; float t5_1 = a[i1+5];
            float t0_2 = a[i2];   float t1_2 = a[i2+1]; float t2_2 = a[i2+2]; float t3_2 = a[i2+3]; float t4_2 = a[i2+4]; float t5_2 = a[i2+5];
            float t0_3 = a[i3];   float t1_3 = a[i3+1]; float t2_3 = a[i3+2]; float t3_3 = a[i3+3]; float t4_3 = a[i3+4]; float t5_3 = a[i3+5];

            // Store results
            a[i0]   = t1_0 * t0_0; a[i0+1] = t2_0 * t1_0; a[i0+2] = t3_0 * t2_0; a[i0+3] = t4_0 * t3_0; a[i0+4] = t5_0 * t4_0;
            a[i1]   = t1_1 * t0_1; a[i1+1] = t2_1 * t1_1; a[i1+2] = t3_1 * t2_1; a[i1+3] = t4_1 * t3_1; a[i1+4] = t5_1 * t4_1;
            a[i2]   = t1_2 * t0_2; a[i2+1] = t2_2 * t1_2; a[i2+2] = t3_2 * t2_2; a[i2+3] = t4_2 * t3_2; a[i2+4] = t5_2 * t4_2;
            a[i3]   = t1_3 * t0_3; a[i3+1] = t2_3 * t1_3; a[i3+2] = t3_3 * t2_3; a[i3+3] = t4_3 * t3_3; a[i3+4] = t5_3 * t4_3;
        }

        // Scalar cleanup for remaining groups
        for (; g < num_groups; g++) {
            int i = g * 5;
            float t0 = a[i];
            float t1 = a[i + 1];
            float t2 = a[i + 2];
            float t3 = a[i + 3];
            float t4 = a[i + 4];
            float t5 = a[i + 5];

            a[i]     = t1 * t0;
            a[i + 1] = t2 * t1;
            a[i + 2] = t3 * t2;
            a[i + 3] = t4 * t3;
            a[i + 4] = t5 * t4;
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
