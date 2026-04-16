#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s292(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Handle wrap-around cases for i=0 and i=1
        a[0] = (b[0] + b[LEN_1D-1] + b[LEN_1D-2]) * (float)0.333;
        a[1] = (b[1] + b[0] + b[LEN_1D-1]) * (float)0.333;
        // For i>=2, access is sequential: b[i], b[i-1], b[i-2]
        for (int i = 2; i < LEN_1D; i++) {
            a[i] = (b[i] + b[i-1] + b[i-2]) * (float)0.333;
        }
    }
}

        #include <stdint.h>

void vectorized_s292(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Handle wrap-around cases for i=0 and i=1
        a[0] = (b[0] + b[LEN_1D-1] + b[LEN_1D-2]) * (float)0.333;
        a[1] = (b[1] + b[0] + b[LEN_1D-1]) * (float)0.333;

        // For i>=2, access is sequential: b[i], b[i-1], b[i-2]
        // Vectorize using width of 8 floats
        int i = 2;
        int limit = LEN_1D - 8;

        for (; i <= limit; i += 8) {
            // b[i..i+7]
            float b0 = b[i+0], b1 = b[i+1], b2 = b[i+2], b3 = b[i+3];
            float b4 = b[i+4], b5 = b[i+5], b6 = b[i+6], b7 = b[i+7];
            // b[i-1..i+6]
            float bm1_0 = b[i-1], bm1_1 = b[i+0], bm1_2 = b[i+1], bm1_3 = b[i+2];
            float bm1_4 = b[i+3], bm1_5 = b[i+4], bm1_6 = b[i+5], bm1_7 = b[i+6];
            // b[i-2..i+5]
            float bm2_0 = b[i-2], bm2_1 = b[i-1], bm2_2 = b[i+0], bm2_3 = b[i+1];
            float bm2_4 = b[i+2], bm2_5 = b[i+3], bm2_6 = b[i+4], bm2_7 = b[i+5];

            a[i+0] = (b0 + bm1_0 + bm2_0) * (float)0.333;
            a[i+1] = (b1 + bm1_1 + bm2_1) * (float)0.333;
            a[i+2] = (b2 + bm1_2 + bm2_2) * (float)0.333;
            a[i+3] = (b3 + bm1_3 + bm2_3) * (float)0.333;
            a[i+4] = (b4 + bm1_4 + bm2_4) * (float)0.333;
            a[i+5] = (b5 + bm1_5 + bm2_5) * (float)0.333;
            a[i+6] = (b6 + bm1_6 + bm2_6) * (float)0.333;
            a[i+7] = (b7 + bm1_7 + bm2_7) * (float)0.333;
        }

        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            a[i] = (b[i] + b[i-1] + b[i-2]) * (float)0.333;
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
            int iterations = 5; int LEN_1D = n; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, n, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar));
                s292(iterations, LEN_1D, a_scalar, b_scalar); vectorized_s292(iterations, LEN_1D, a_vector, b_vector);
                for (int i = 0; i < n; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((b_scalar[i]) - (b_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter b on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
