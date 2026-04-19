#include <stddef.h>

void vectorized_s341(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < iterations; nl++) {
        int j = -1;
        int mask[LEN_1D];
        int write_idx[LEN_1D];
        
        // Vectorizable mask computation
        for (int i = 0; i < LEN_1D; i++) {
            mask[i] = (b[i] > 0.0f);
        }
        
        // Prefix sum - remains scalar
        for (int i = 0; i < LEN_1D; i++) {
            j += mask[i];
            write_idx[i] = j;
        }
        
        // Vectorizable scatter
        for (int i = 0; i < LEN_1D; i++) {
            if (mask[i]) {
                a[write_idx[i]] = b[i];
            }
        }
    }
}
