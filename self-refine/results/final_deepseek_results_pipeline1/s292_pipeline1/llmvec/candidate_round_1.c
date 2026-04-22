#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s292(int iterations,int LEN_1D, float* a,float* b)
{
    int im1, im2;
    for (int nl = 0; nl < iterations; nl++) {
        im1 = LEN_1D-1;
        im2 = LEN_1D-2;
        for (int i = 0; i < LEN_1D; i++) {
            a[i] = (b[i] + b[im1] + b[im2]) * (float).333;
            im2 = im1;
            im1 = i;
        }
    }
}

        #include <stddef.h>

void vectorized_s292(int iterations, int LEN_1D, float* a, float* b) {
    if (LEN_1D < 3) {
        // Handle small arrays with scalar code
        for (int nl = 0; nl < iterations; nl++) {
            int im1 = LEN_1D - 1;
            int im2 = LEN_1D - 2;
            for (int i = 0; i < LEN_1D; i++) {
                a[i] = (b[i] + b[im1] + b[im2]) * 0.333f;
                im2 = im1;
                im1 = i;
            }
        }
        return;
    }

    for (int nl = 0; nl < iterations; nl++) {
        // First two iterations handled separately due to wrap-around
        int im1 = LEN_1D - 1;
        int im2 = LEN_1D - 2;

        // i = 0
        a[0] = (b[0] + b[im1] + b[im2]) * 0.333f;
        im2 = im1;
        im1 = 0;

        // i = 1
        a[1] = (b[1] + b[im1] + b[im2]) * 0.333f;
        im2 = im1;
        im1 = 1;

        // Main vectorizable loop: i from 2 to LEN_1D-1
        // At this point im1 = 1, im2 = 0, and for i >= 2:
        // im1 = i-1, im2 = i-2
        int i = 2;
        #pragma clang loop vectorize(enable)
        for (; i < LEN_1D; i++) {
            a[i] = (b[i] + b[i-1] + b[i-2]) * 0.333f;
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
                s292(iterations, LEN_1D, a_scalar, b_scalar); vectorized_s292(iterations, LEN_1D, a_vector, b_vector);
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
