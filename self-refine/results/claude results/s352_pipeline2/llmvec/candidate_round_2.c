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
        float4 vsum0 = {0.0f, 0.0f, 0.0f, 0.0f};
        float4 vsum1 = {0.0f, 0.0f, 0.0f, 0.0f};
        float4 vsum2 = {0.0f, 0.0f, 0.0f, 0.0f};
        float4 vsum3 = {0.0f, 0.0f, 0.0f, 0.0f};
        float4 vsum4 = {0.0f, 0.0f, 0.0f, 0.0f};

        /* Process 4 groups of 5 at a time = 20 elements per iteration */
        int limit = (LEN_1D / 20) * 20;
        int i = 0;

        for (; i < limit; i += 20) {
            float4 a0 = {a[i],   a[i+5],  a[i+10], a[i+15]};
            float4 a1 = {a[i+1], a[i+6],  a[i+11], a[i+16]};
            float4 a2 = {a[i+2], a[i+7],  a[i+12], a[i+17]};
            float4 a3 = {a[i+3], a[i+8],  a[i+13], a[i+18]};
            float4 a4 = {a[i+4], a[i+9],  a[i+14], a[i+19]};

            float4 b0 = {b[i],   b[i+5],  b[i+10], b[i+15]};
            float4 b1 = {b[i+1], b[i+6],  b[i+11], b[i+16]};
            float4 b2 = {b[i+2], b[i+7],  b[i+12], b[i+17]};
            float4 b3 = {b[i+3], b[i+8],  b[i+13], b[i+18]};
            float4 b4 = {b[i+4], b[i+9],  b[i+14], b[i+19]};

            vsum0 += a0 * b0;
            vsum1 += a1 * b1;
            vsum2 += a2 * b2;
            vsum3 += a3 * b3;
            vsum4 += a4 * b4;
        }

        /* Reduce vector accumulators into scalars */
        float sum0 = vsum0[0] + vsum0[1] + vsum0[2] + vsum0[3];
        float sum1 = vsum1[0] + vsum1[1] + vsum1[2] + vsum1[3];
        float sum2 = vsum2[0] + vsum2[1] + vsum2[2] + vsum2[3];
        float sum3 = vsum3[0] + vsum3[1] + vsum3[2] + vsum3[3];
        float sum4 = vsum4[0] + vsum4[1] + vsum4[2] + vsum4[3];

        float total = sum0 + sum1 + sum2 + sum3 + sum4;

        /* Scalar tail: handle remaining elements in groups of 5 */
        for (; i + 4 < LEN_1D; i += 5) {
            total += a[i]     * b[i];
            total += a[i + 1] * b[i + 1];
            total += a[i + 2] * b[i + 2];
            total += a[i + 3] * b[i + 3];
            total += a[i + 4] * b[i + 4];
        }

        /* Handle any truly remaining elements (if LEN_1D not multiple of 5) */
        for (; i < LEN_1D; i++) {
            total += a[i] * b[i];
        }

        dot = total;
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
