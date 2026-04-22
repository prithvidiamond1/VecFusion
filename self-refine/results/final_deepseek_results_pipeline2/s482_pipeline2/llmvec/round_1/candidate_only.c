#include <stdint.h>

void vectorized_s482(int iterations, int LEN_1D, float* a, float *b, float *c) {
    for (int nl = 0; nl < iterations; nl++) {
        // First, find where the break condition occurs
        int break_idx = LEN_1D;
        for (int i = 0; i < LEN_1D; i++) {
            if (c[i] > b[i]) {
                break_idx = i;
                break;
            }
        }

        // Process all iterations before the break point (these can be vectorized)
        // Note: break_idx iteration is included because the break happens AFTER the update
        int limit = break_idx + 1;
        if (limit > LEN_1D) limit = LEN_1D;
        
        // Vectorizable loop with explicit bounds
        int i = 0;
        #if defined(__clang__) && defined(__SSE__)
        // Process 4 elements at a time using vector extensions
        for (; i + 3 < limit; i += 4) {
            float va0 = a[i], va1 = a[i+1], va2 = a[i+2], va3 = a[i+3];
            float vb0 = b[i], vb1 = b[i+1], vb2 = b[i+2], vb3 = b[i+3];
            float vc0 = c[i], vc1 = c[i+1], vc2 = c[i+2], vc3 = c[i+3];
            a[i]   = va0   + vb0   * vc0;
            a[i+1] = va1   + vb1   * vc1;
            a[i+2] = va2   + vb2   * vc2;
            a[i+3] = va3   + vb3   * vc3;
        }
        #endif
        // Scalar cleanup for remaining elements
        for (; i < limit; i++) {
            a[i] += b[i] * c[i];
        }
    }
}
