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
    float s;
    for (int nl = 0; nl < iterations * 2; nl++) {
        s = 0.0f;

        // Precompute s for each index: s at index i = 2.0f * (i+1)
        // a[i] = s * b[i] where s = 2*(i+1)
        // We can vectorize by precomputing the multiplier array inline

        int i = 0;
        float s_val = 0.0f;

        // Unroll by 8
        for (; i <= LEN_1D - 8; i += 8) {
            float s0 = s_val + 2.0f;
            float s1 = s_val + 4.0f;
            float s2 = s_val + 6.0f;
            float s3 = s_val + 8.0f;
            float s4 = s_val + 10.0f;
            float s5 = s_val + 12.0f;
            float s6 = s_val + 14.0f;
            float s7 = s_val + 16.0f;

            a[i+0] = s0 * b[i+0];
            a[i+1] = s1 * b[i+1];
            a[i+2] = s2 * b[i+2];
            a[i+3] = s3 * b[i+3];
            a[i+4] = s4 * b[i+4];
            a[i+5] = s5 * b[i+5];
            a[i+6] = s6 * b[i+6];
            a[i+7] = s7 * b[i+7];

            s_val += 16.0f;
        }

        // Scalar tail
        for (; i < LEN_1D; i++) {
            s_val += 2.0f;
            a[i] = s_val * b[i];
        }

        (void)s;
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
