#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s352(int iterations, int LEN_1D, float* a, float* b)
{
    float dot = 0.0f;
    for (int nl = 0; nl < 8 * iterations; nl++) {
        float sum0 = 0.0f, sum1 = 0.0f, sum2 = 0.0f, sum3 = 0.0f, sum4 = 0.0f;
        for (int i = 0; i < LEN_1D; i += 5) {
            sum0 += a[i]     * b[i];
            sum1 += a[i + 1] * b[i + 1];
            sum2 += a[i + 2] * b[i + 2];
            sum3 += a[i + 3] * b[i + 3];
            sum4 += a[i + 4] * b[i + 4];
        }
        dot = sum0 + sum1 + sum2 + sum3 + sum4;
    }
    return dot;
}

        #include <stdint.h>

typedef float float4 __attribute__((vector_size(16)));

float vectorized_s352(int iterations, int LEN_1D, float* a, float* b)
{
    float dot = 0.0f;
    for (int nl = 0; nl < 8 * iterations; nl++) {
        float4 vsum = {0.0f, 0.0f, 0.0f, 0.0f};

        int limit = (LEN_1D / 4) * 4;
        int i = 0;

        for (; i < limit; i += 4) {
            float4 va, vb;
            va[0] = a[i];   va[1] = a[i+1]; va[2] = a[i+2]; va[3] = a[i+3];
            vb[0] = b[i];   vb[1] = b[i+1]; vb[2] = b[i+2]; vb[3] = b[i+3];
            vsum += va * vb;
        }

        float total = vsum[0] + vsum[1] + vsum[2] + vsum[3];

        for (; i < LEN_1D; i++) {
            total += a[i] * b[i];
        }

        dot += total;
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
