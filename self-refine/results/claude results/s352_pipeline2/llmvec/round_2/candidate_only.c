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
