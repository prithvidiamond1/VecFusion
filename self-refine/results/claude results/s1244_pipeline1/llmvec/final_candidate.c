#include <stdint.h>

void vectorized_s1244(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    typedef float float4 __attribute__((vector_size(16)));

    int vec_len = (LEN_1D - 1);

    for (int nl = 0; nl < iterations; nl++) {
        // First pass: compute a[i] for i in [0, LEN_1D-1)
        int i = 0;
        for (; i <= vec_len - 4; i += 4) {
            float4 bi, ci, ai;
            // Load 4 floats
            __builtin_memcpy(&bi, b + i, sizeof(float4));
            __builtin_memcpy(&ci, c + i, sizeof(float4));
            ai = bi + ci * ci + bi * bi + ci;
            __builtin_memcpy(a + i, &ai, sizeof(float4));
        }
        // Scalar tail for a[i]
        for (; i < vec_len; i++) {
            a[i] = b[i] + c[i] * c[i] + b[i] * b[i] + c[i];
        }
        // We also need a[LEN_1D-1] for the d computation below if needed
        // Actually d[i] = a[i] + a[i+1] for i in [0, LEN_1D-2]
        // a[LEN_1D-1] is accessed as a[i+1] when i = LEN_1D-2
        // But the loop only computes a[i] for i < LEN_1D-1, so a[LEN_1D-1] is not set here.
        // The original code also doesn't set a[LEN_1D-1], so we leave it as-is.

        // Second pass: compute d[i] = a[i] + a[i+1] for i in [0, LEN_1D-2)
        // Note: vec_len = LEN_1D - 1, so i goes from 0 to LEN_1D-2 inclusive
        i = 0;
        for (; i <= vec_len - 1 - 4; i += 4) {
            float4 ai, ai1, di;
            __builtin_memcpy(&ai, a + i, sizeof(float4));
            __builtin_memcpy(&ai1, a + i + 1, sizeof(float4));
            di = ai + ai1;
            __builtin_memcpy(d + i, &di, sizeof(float4));
        }
        // Scalar tail for d[i]
        for (; i < vec_len; i++) {
            d[i] = a[i] + a[i + 1];
        }
    }
}
