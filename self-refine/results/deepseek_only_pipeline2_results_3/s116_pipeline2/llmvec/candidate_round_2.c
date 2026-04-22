#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s116(int iterations, int LEN_1D, float* a) {
    for (int nl = 0; nl < iterations*10; nl++) {
        // Process in forward order with temporary storage
        for (int i = 0; i < LEN_1D - 5; i += 5) {
            // Load all original values before any updates
            float orig_i = a[i];
            float orig_i1 = a[i + 1];
            float orig_i2 = a[i + 2];
            float orig_i3 = a[i + 3];
            float orig_i4 = a[i + 4];
            float orig_i5 = a[i + 5];

            // Compute all results using original values
            a[i] = orig_i1 * orig_i;
            a[i + 1] = orig_i2 * orig_i1;
            a[i + 2] = orig_i3 * orig_i2;
            a[i + 3] = orig_i4 * orig_i3;
            a[i + 4] = orig_i5 * orig_i4;
        }
    }
}

        #include <stddef.h>

void vectorized_s116(int iterations, int LEN_1D, float* a) {
    if (LEN_1D < 6) return;

    int outer_iterations = iterations * 10;
    int vector_limit = LEN_1D - 6;

    for (int nl = 0; nl < outer_iterations; nl++) {
        int i = 0;

        // Vector-friendly main loop: process 5 outputs per iteration using 6 original values
        for (; i <= vector_limit; i += 5) {
            // Load 6 consecutive original values
            float orig_i = a[i];
            float orig_i1 = a[i + 1];
            float orig_i2 = a[i + 2];
            float orig_i3 = a[i + 3];
            float orig_i4 = a[i + 4];
            float orig_i5 = a[i + 5];

            // Compute 5 outputs using only these original values
            a[i] = orig_i1 * orig_i;
            a[i + 1] = orig_i2 * orig_i1;
            a[i + 2] = orig_i3 * orig_i2;
            a[i + 3] = orig_i4 * orig_i3;
            a[i + 4] = orig_i5 * orig_i4;
        }

        // Scalar cleanup for remaining elements (if any)
        int scalar_limit = LEN_1D - 5;
        for (; i < scalar_limit; i++) {
            float orig_i = a[i];
            float orig_i1 = a[i + 1];
            a[i] = orig_i1 * orig_i;
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
