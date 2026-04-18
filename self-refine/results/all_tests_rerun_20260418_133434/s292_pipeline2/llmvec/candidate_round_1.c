#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s292(int iterations,int LEN_1D, float* a,float* b)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Precompute the two wrap-around indices for the first two elements
        float b_im1 = b[LEN_1D-1];
        float b_im2 = b[LEN_1D-2];

        // Handle i=0 separately (uses b[LEN_1D-1] and b[LEN_1D-2])
        a[0] = (b[0] + b_im1 + b_im2) * (float).333;

        // Handle i=1 separately (uses b[0] and b[LEN_1D-1])
        float b_i_minus_1 = b[0];
        a[1] = (b[1] + b_i_minus_1 + b_im1) * (float).333;

        // Main vectorizable loop for i=2..LEN_1D-1
        // No loop-carried dependency: uses b[i], b[i-1], b[i-2]
        for (int i = 2; i < LEN_1D; i++) {
            a[i] = (b[i] + b[i-1] + b[i-2]) * (float).333;
        }
    }
}

        #include <stdint.h>

void vectorized_s292(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < iterations; nl++) {
        float b_im1 = b[LEN_1D-1];
        float b_im2 = b[LEN_1D-2];

        a[0] = (b[0] + b_im1 + b_im2) * (float).333;

        float b_i_minus_1 = b[0];
        a[1] = (b[1] + b_i_minus_1 + b_im1) * (float).333;

        // Vectorize the main loop i=2..LEN_1D-1
        // a[i] = (b[i] + b[i-1] + b[i-2]) * 0.333f
        // No loop-carried dependency since we only read from b[]

        int i = 2;
        int n = LEN_1D;

        // Vector width: 8 floats
        #define VW 8
        typedef float float8 __attribute__((vector_size(VW * sizeof(float))));

        int limit = n - VW;
        for (; i <= limit; i += VW) {
            float8 v0, v1, v2, vr;
            // b[i..i+VW-1]
            v0 = (float8){ b[i],   b[i+1], b[i+2], b[i+3], b[i+4], b[i+5], b[i+6], b[i+7] };
            // b[i-1..i+VW-2]
            v1 = (float8){ b[i-1], b[i],   b[i+1], b[i+2], b[i+3], b[i+4], b[i+5], b[i+6] };
            // b[i-2..i+VW-3]
            v2 = (float8){ b[i-2], b[i-1], b[i],   b[i+1], b[i+2], b[i+3], b[i+4], b[i+5] };

            vr = (v0 + v1 + v2) * (float8){ 0.333f, 0.333f, 0.333f, 0.333f,
                                             0.333f, 0.333f, 0.333f, 0.333f };

            a[i]   = vr[0];
            a[i+1] = vr[1];
            a[i+2] = vr[2];
            a[i+3] = vr[3];
            a[i+4] = vr[4];
            a[i+5] = vr[5];
            a[i+6] = vr[6];
            a[i+7] = vr[7];
        }

        // Scalar cleanup tail
        for (; i < n; i++) {
            a[i] = (b[i] + b[i-1] + b[i-2]) * (float).333;
        }
        #undef VW
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
