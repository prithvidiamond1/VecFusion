#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s352(int iterations,int LEN_1D, float* a, float *b)
{
    float dot;
    for (int nl = 0; nl < 8*iterations; nl++) {
        dot = (float)0.;
        for (int i = 0; i < LEN_1D; i += 5) {
            dot = dot + a[i] * b[i] + a[i + 1] * b[i + 1] + a[i + 2]
                * b[i + 2] + a[i + 3] * b[i + 3] + a[i + 4] * b[i + 4];
        }
    }
    return dot;
}

        #include <stdint.h>

float vectorized_s352(int iterations, int LEN_1D, float* a, float* b)
{
    float dot;
    for (int nl = 0; nl < 8 * iterations; nl++) {
        dot = 0.0f;

        int i = 0;
        int limit = LEN_1D - (LEN_1D % 20);

        float acc0 = 0.0f, acc1 = 0.0f, acc2 = 0.0f, acc3 = 0.0f;

        for (; i < limit; i += 20) {
            acc0 += a[i + 0] * b[i + 0]
                  + a[i + 1] * b[i + 1]
                  + a[i + 2] * b[i + 2]
                  + a[i + 3] * b[i + 3]
                  + a[i + 4] * b[i + 4];
            acc1 += a[i + 5] * b[i + 5]
                  + a[i + 6] * b[i + 6]
                  + a[i + 7] * b[i + 7]
                  + a[i + 8] * b[i + 8]
                  + a[i + 9] * b[i + 9];
            acc2 += a[i + 10] * b[i + 10]
                  + a[i + 11] * b[i + 11]
                  + a[i + 12] * b[i + 12]
                  + a[i + 13] * b[i + 13]
                  + a[i + 14] * b[i + 14];
            acc3 += a[i + 15] * b[i + 15]
                  + a[i + 16] * b[i + 16]
                  + a[i + 17] * b[i + 17]
                  + a[i + 18] * b[i + 18]
                  + a[i + 19] * b[i + 19];
        }

        dot = acc0 + acc1 + acc2 + acc3;

        /* Safe scalar cleanup: step by 1 to handle any remainder */
        for (; i < LEN_1D; i++) {
            dot += a[i] * b[i];
        }
    }
    return dot;
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
            int iterations = 5; int LEN_1D = n; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float ret_scalar; float ret_vector;

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, n, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar));
                ret_scalar = s352(iterations, LEN_1D, a_scalar, b_scalar); ret_vector = vectorized_s352(iterations, LEN_1D, a_vector, b_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
    fprintf(stderr, "Return mismatch on trial %d\n", trial);
    return 2;
}
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
