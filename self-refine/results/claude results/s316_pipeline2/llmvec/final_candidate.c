#include <stdint.h>

typedef float float4 __attribute__((vector_size(16)));

float vectorized_s316(int iterations, int LEN_1D, float* a)
{
    float x;
    for (int nl = 0; nl < iterations * 5; nl++) {
        // Initialize 4-wide SIMD accumulators with a[0]
        float4 vmin = {a[0], a[0], a[0], a[0]};

        int i;
        // Process 16 elements at a time using 4 vector lanes
        for (i = 1; i + 15 < LEN_1D; i += 16) {
            float4 v0 = {a[i],    a[i+1],  a[i+2],  a[i+3]};
            float4 v1 = {a[i+4],  a[i+5],  a[i+6],  a[i+7]};
            float4 v2 = {a[i+8],  a[i+9],  a[i+10], a[i+11]};
            float4 v3 = {a[i+12], a[i+13], a[i+14], a[i+15]};

            float4 m0 = __builtin_elementwise_min(vmin, v0);
            float4 m1 = __builtin_elementwise_min(v1, v2);
            float4 m2 = __builtin_elementwise_min(m0, m1);
            float4 m3 = __builtin_elementwise_min(m2, v3);
            vmin = m3;
        }

        // Process remaining groups of 4
        for (; i + 3 < LEN_1D; i += 4) {
            float4 v = {a[i], a[i+1], a[i+2], a[i+3]};
            vmin = __builtin_elementwise_min(vmin, v);
        }

        // Reduce the 4-lane vector to scalar
        float x0 = vmin[0];
        float x1 = vmin[1];
        float x2 = vmin[2];
        float x3 = vmin[3];

        // Scalar tail
        for (; i < LEN_1D; i++) {
            x0 = x0 < a[i] ? x0 : a[i];
        }

        x0 = x0 < x1 ? x0 : x1;
        x2 = x2 < x3 ? x2 : x3;
        x = x0 < x2 ? x0 : x2;
    }
    return x;
}
