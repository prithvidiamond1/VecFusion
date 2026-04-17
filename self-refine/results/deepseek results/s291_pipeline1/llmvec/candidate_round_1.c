#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s291(int iterations,int LEN_1D, float* a,float* b)
{
    int im1;
    for (int nl = 0; nl < 2*iterations; nl++) {
        im1 = LEN_1D-1;
        for (int i = 0; i < LEN_1D; i++) {
            a[i] = (b[i] + b[im1]) * (float).5;
            im1 = i;
        }
    }
}

        #include <stdint.h>

void vectorized_s291(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < 2 * iterations; nl++) {
        // First element: uses b[LEN_1D-1] as im1
        a[0] = (b[0] + b[LEN_1D - 1]) * 0.5f;

        // Remaining elements: a[i] = (b[i] + b[i-1]) * 0.5
        // This is a simple shift-and-add, vectorizable since reads are from b (not a)
        int i = 1;

        // Unroll by 8 for vectorization
        for (; i <= LEN_1D - 8; i += 8) {
            a[i + 0] = (b[i + 0] + b[i - 1]) * 0.5f;
            a[i + 1] = (b[i + 1] + b[i + 0]) * 0.5f;
            a[i + 2] = (b[i + 2] + b[i + 1]) * 0.5f;
            a[i + 3] = (b[i + 3] + b[i + 2]) * 0.5f;
            a[i + 4] = (b[i + 4] + b[i + 3]) * 0.5f;
            a[i + 5] = (b[i + 5] + b[i + 4]) * 0.5f;
            a[i + 6] = (b[i + 6] + b[i + 5]) * 0.5f;
            a[i + 7] = (b[i + 7] + b[i + 6]) * 0.5f;
        }

        // Scalar tail
        for (; i < LEN_1D; i++) {
            a[i] = (b[i] + b[i - 1]) * 0.5f;
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
