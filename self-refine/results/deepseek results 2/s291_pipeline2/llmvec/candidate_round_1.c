#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s291(int iterations,int LEN_1D, float* a,float* b)
{
    for (int nl = 0; nl < 2*iterations; nl++) {
        // Handle first element separately (wrap-around case)
        a[0] = (b[0] + b[LEN_1D-1]) * (float).5;

        // Main vectorizable loop: i from 1 to LEN_1D-1
        // No loop-carried dependency since we use b[i-1] directly
        for (int i = 1; i < LEN_1D; i++) {
            a[i] = (b[i] + b[i-1]) * (float).5;
        }
    }
}

        #include <stdint.h>

void vectorized_s291(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < 2 * iterations; nl++) {
        // Handle first element separately (wrap-around case)
        a[0] = (b[0] + b[LEN_1D - 1]) * 0.5f;

        // Vectorize the main loop using compiler vector types
        int i = 1;

        // Vector width: 8 floats at a time
        #define VEC_WIDTH 8
        typedef float float8 __attribute__((vector_size(8 * sizeof(float))));

        int vec_end = 1 + ((LEN_1D - 1) / VEC_WIDTH) * VEC_WIDTH;

        for (; i < vec_end; i += VEC_WIDTH) {
            float8 bi, bim1, result;
            // Load b[i..i+7]
            __builtin_memcpy(&bi, &b[i], sizeof(float8));
            // Load b[i-1..i+6]
            __builtin_memcpy(&bim1, &b[i - 1], sizeof(float8));
            result = (bi + bim1) * (float8){0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f};
            __builtin_memcpy(&a[i], &result, sizeof(float8));
        }

        // Scalar cleanup tail
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
