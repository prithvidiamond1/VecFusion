#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s291(int iterations,int LEN_1D, float* a,float* b)
{
    int im1;
    float temp;

    for (int nl = 0; nl < 2*iterations; nl++) {
        im1 = LEN_1D-1;

        // Handle first iteration separately to break dependency
        a[0] = (b[0] + b[im1]) * (float).5;

        // Main loop - reordered to remove loop-carried dependency
        for (int i = 1; i < LEN_1D; i++) {
            // Store current im1 value before updating
            temp = b[i-1];
            // Update a[i] using stored value
            a[i] = (b[i] + temp) * (float).5;
        }
    }
}

        #include <stddef.h>

void vectorized_s291(int iterations, int LEN_1D, float* a, float* b) {
    int im1;
    float temp;

    for (int nl = 0; nl < 2 * iterations; nl++) {
        im1 = LEN_1D - 1;

        // Handle first iteration separately
        a[0] = (b[0] + b[im1]) * (float).5;

        // Vectorized main loop
        int i = 1;
        // Process in chunks of 4 for simple vectorization
        for (; i + 3 < LEN_1D; i += 4) {
            // Load b[i] values
            float b0 = b[i];
            float b1 = b[i + 1];
            float b2 = b[i + 2];
            float b3 = b[i + 3];

            // Load b[i-1] values (previous elements)
            float bm1_0 = b[i - 1];
            float bm1_1 = b[i];
            float bm1_2 = b[i + 1];
            float bm1_3 = b[i + 2];

            // Compute averages
            a[i] = (b0 + bm1_0) * (float).5;
            a[i + 1] = (b1 + bm1_1) * (float).5;
            a[i + 2] = (b2 + bm1_2) * (float).5;
            a[i + 3] = (b3 + bm1_3) * (float).5;
        }

        // Scalar cleanup for remaining elements
        for (; i < LEN_1D; i++) {
            temp = b[i - 1];
            a[i] = (b[i] + temp) * (float).5;
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar));
                s291(iterations, LEN_1D, a_scalar, b_scalar); vectorized_s291(iterations, LEN_1D, a_vector, b_vector);
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
