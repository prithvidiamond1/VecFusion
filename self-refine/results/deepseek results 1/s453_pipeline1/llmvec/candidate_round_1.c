#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s453(int iterations,int LEN_1D, float* a, float *b)
{
    float s;
    for (int nl = 0; nl < iterations*2; nl++) {
        s = 0.;
        for (int i = 0; i < LEN_1D; i++) {
            s += (float)2.;
            a[i] = s * b[i];
        }
    }
}

        #include <stdint.h>

void vectorized_s453(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < iterations * 2; nl++) {
        // s starts at 0, increments by 2 each iteration
        // so at index i, s = 2*(i+1) = 2*i + 2
        // a[i] = s * b[i] = (2*(i+1)) * b[i]

        int i = 0;

        // Unroll by 4 for vector-friendly code
        for (; i <= LEN_1D - 4; i += 4) {
            float s0 = (float)(2 * (i + 1));
            float s1 = (float)(2 * (i + 2));
            float s2 = (float)(2 * (i + 3));
            float s3 = (float)(2 * (i + 4));
            a[i + 0] = s0 * b[i + 0];
            a[i + 1] = s1 * b[i + 1];
            a[i + 2] = s2 * b[i + 2];
            a[i + 3] = s3 * b[i + 3];
        }

        // Scalar tail
        for (; i < LEN_1D; i++) {
            float s = (float)(2 * (i + 1));
            a[i] = s * b[i];
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
                s453(iterations, LEN_1D, a_scalar, b_scalar); vectorized_s453(iterations, LEN_1D, a_vector, b_vector);
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
