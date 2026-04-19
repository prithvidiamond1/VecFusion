#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s116(int iterations, int LEN_1D, float* a) {
    for (int nl = 0; nl < iterations * 10; nl++) {
        // Process all elements except the last 5 in a single stride-1 loop
        // This allows vectorization since each iteration only uses original values
        for (int i = 0; i < LEN_1D - 5; i++) {
            a[i] = a[i + 1] * a[i];
        }

        // Handle the remaining 5 elements with their specific update pattern
        // This small loop won't significantly impact performance
        for (int i = LEN_1D - 5; i < LEN_1D - 4; i++) {
            float t1 = a[i + 1];
            float t2 = a[i + 2];
            float t3 = a[i + 3];
            float t4 = a[i + 4];
            float t5 = a[i + 5];

            a[i] = t1 * a[i];
            a[i + 1] = t2 * t1;
            a[i + 2] = t3 * t2;
            a[i + 3] = t4 * t3;
            a[i + 4] = t5 * t4;
        }
    }
}

        #include <stddef.h>

void vectorized_s116(int iterations, int LEN_1D, float* a) {
    for (int nl = 0; nl < iterations * 10; nl++) {
        // Vectorized main loop: process 4 elements per iteration
        int i = 0;
        for (; i <= LEN_1D - 5 - 4; i += 4) {
            // Load original values a[i] through a[i+3]
            float b0 = a[i];
            float b1 = a[i + 1];
            float b2 = a[i + 2];
            float b3 = a[i + 3];

            // Load shifted values a[i+1] through a[i+4]
            float s1 = a[i + 1];
            float s2 = a[i + 2];
            float s3 = a[i + 3];
            float s4 = a[i + 4];

            // Compute updates
            a[i] = s1 * b0;
            a[i + 1] = s2 * b1;
            a[i + 2] = s3 * b2;
            a[i + 3] = s4 * b3;
        }

        // Scalar cleanup for remaining elements (except last 5)
        for (; i < LEN_1D - 5; i++) {
            a[i] = a[i + 1] * a[i];
        }

        // Handle the remaining 5 elements with their specific update pattern
        if (LEN_1D >= 5) {
            int i = LEN_1D - 5;
            float t1 = a[i + 1];
            float t2 = a[i + 2];
            float t3 = a[i + 3];
            float t4 = a[i + 4];
            float t5 = a[LEN_1D - 1];

            a[i] = t1 * a[i];
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
