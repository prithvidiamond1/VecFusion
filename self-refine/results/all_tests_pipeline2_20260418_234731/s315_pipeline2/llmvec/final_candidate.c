#include <stddef.h>

float vectorized_s315(int iterations, int LEN_1D, float* a) {
    // Initialize array (vectorizable)
    for (int i = 0; i < LEN_1D; i++) {
        a[i] = (i * 7) % LEN_1D;
    }

    float x;
    int index;
    float chksum = 0.0f;

    // Outer loop over iterations
    for (int nl = 0; nl < iterations; nl++) {
        x = a[0];
        index = 0;

        // First loop: find max value (vectorizable reduction)
        float max_val = a[0];
        int i = 1;
        
        // Vectorized max reduction
        for (; i + 3 < LEN_1D; i += 4) {
            float val0 = a[i];
            float val1 = a[i + 1];
            float val2 = a[i + 2];
            float val3 = a[i + 3];
            
            if (val0 > max_val) max_val = val0;
            if (val1 > max_val) max_val = val1;
            if (val2 > max_val) max_val = val2;
            if (val3 > max_val) max_val = val3;
        }
        
        // Scalar tail
        for (; i < LEN_1D; ++i) {
            float val = a[i];
            if (val > max_val) max_val = val;
        }

        // Second loop: find last index of max value (vectorizable)
        int max_idx = 0;
        i = 0;
        
        // Vectorized index tracking
        for (; i + 3 < LEN_1D; i += 4) {
            int idx0 = i;
            int idx1 = i + 1;
            int idx2 = i + 2;
            int idx3 = i + 3;
            
            int is_max0 = a[idx0] == max_val;
            int is_max1 = a[idx1] == max_val;
            int is_max2 = a[idx2] == max_val;
            int is_max3 = a[idx3] == max_val;
            
            if (is_max0) max_idx = idx0;
            if (is_max1) max_idx = idx1;
            if (is_max2) max_idx = idx2;
            if (is_max3) max_idx = idx3;
        }
        
        // Scalar tail
        for (; i < LEN_1D; ++i) {
            if (a[i] == max_val) max_idx = i;
        }

        x = max_val;
        index = max_idx;

        // Compute chksum to maintain exact memory access pattern
        chksum = x + (float)index;
        (void)chksum;
    }

    return index + x + 1;
}
