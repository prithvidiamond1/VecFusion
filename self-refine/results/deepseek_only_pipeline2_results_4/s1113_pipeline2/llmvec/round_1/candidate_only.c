#include <stddef.h>

void vectorized_s1113(int iterations, int LEN_1D, float* a, float* b) {
    int mid = LEN_1D / 2;
    
    for (int nl = 0; nl < 2 * iterations; nl++) {
        float mid_val = a[mid];
        
        // Vectorize first part: i < mid
        int i = 0;
        for (; i + 3 < mid; i += 4) {
            a[i] = mid_val + b[i];
            a[i + 1] = mid_val + b[i + 1];
            a[i + 2] = mid_val + b[i + 2];
            a[i + 3] = mid_val + b[i + 3];
        }
        // Scalar tail for first part
        for (; i < mid; i++) {
            a[i] = mid_val + b[i];
        }
        
        // Middle element
        a[mid] = mid_val + b[mid];
        
        // Vectorize second part: i > mid
        float updated_mid_val = a[mid];
        i = mid + 1;
        for (; i + 3 < LEN_1D; i += 4) {
            a[i] = updated_mid_val + b[i];
            a[i + 1] = updated_mid_val + b[i + 1];
            a[i + 2] = updated_mid_val + b[i + 2];
            a[i + 3] = updated_mid_val + b[i + 3];
        }
        // Scalar tail for second part
        for (; i < LEN_1D; i++) {
            a[i] = updated_mid_val + b[i];
        }
    }
}
