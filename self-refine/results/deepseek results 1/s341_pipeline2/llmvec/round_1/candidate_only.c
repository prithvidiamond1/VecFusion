#include <string.h>

void vectorized_s341(int iterations, int LEN_1D, float* a, float* b) {
    for (int nl = 0; nl < iterations; nl++) {
        // Allocate prefix array on stack if small, otherwise use a local approach
        // We'll use a VLA for generality
        int prefix[LEN_1D];

        // Vectorize the prefix mask computation (parallel part)
        // Step 1: compute the indicator values in parallel
        int i;
        // Unroll by 8 for vector-friendly code
        int limit8 = (LEN_1D / 8) * 8;
        for (i = 0; i < limit8; i += 8) {
            prefix[i+0] = (b[i+0] > 0.0f) ? 1 : 0;
            prefix[i+1] = (b[i+1] > 0.0f) ? 1 : 0;
            prefix[i+2] = (b[i+2] > 0.0f) ? 1 : 0;
            prefix[i+3] = (b[i+3] > 0.0f) ? 1 : 0;
            prefix[i+4] = (b[i+4] > 0.0f) ? 1 : 0;
            prefix[i+5] = (b[i+5] > 0.0f) ? 1 : 0;
            prefix[i+6] = (b[i+6] > 0.0f) ? 1 : 0;
            prefix[i+7] = (b[i+7] > 0.0f) ? 1 : 0;
        }
        for (; i < LEN_1D; i++) {
            prefix[i] = (b[i] > 0.0f) ? 1 : 0;
        }

        // Step 2: sequential prefix sum (inherently sequential)
        for (i = 1; i < LEN_1D; i++) {
            prefix[i] += prefix[i-1];
        }

        // Step 3: scatter - unroll by 8
        for (i = 0; i < limit8; i += 8) {
            if (b[i+0] > 0.0f) a[prefix[i+0]-1] = b[i+0];
            if (b[i+1] > 0.0f) a[prefix[i+1]-1] = b[i+1];
            if (b[i+2] > 0.0f) a[prefix[i+2]-1] = b[i+2];
            if (b[i+3] > 0.0f) a[prefix[i+3]-1] = b[i+3];
            if (b[i+4] > 0.0f) a[prefix[i+4]-1] = b[i+4];
            if (b[i+5] > 0.0f) a[prefix[i+5]-1] = b[i+5];
            if (b[i+6] > 0.0f) a[prefix[i+6]-1] = b[i+6];
            if (b[i+7] > 0.0f) a[prefix[i+7]-1] = b[i+7];
        }
        for (; i < LEN_1D; i++) {
            if (b[i] > 0.0f) a[prefix[i]-1] = b[i];
        }
    }
}
