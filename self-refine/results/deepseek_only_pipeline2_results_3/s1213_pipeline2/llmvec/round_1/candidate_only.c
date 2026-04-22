#include <stddef.h>

void vectorized_s1213(int iterations, int LEN_1D, float* a, float* b, float* c, float* d) {
    if (LEN_1D <= 2) return;
    
    int loop_bound = LEN_1D - 1;
    
    for (int nl = 0; nl < iterations; nl++) {
        // Vectorized store of a[i+1] * d[i]
        int i = 1;
        for (; i + 3 < loop_bound; i += 4) {
            b[i] = a[i+1] * d[i];
            b[i+1] = a[i+2] * d[i+1];
            b[i+2] = a[i+3] * d[i+2];
            b[i+3] = a[i+4] * d[i+3];
        }
        // Scalar tail for first loop
        for (; i < loop_bound; i++) {
            b[i] = a[i+1] * d[i];
        }
        
        // Vectorized computation of b[i-1] + c[i]
        i = 1;
        for (; i + 3 < loop_bound; i += 4) {
            a[i] = b[i-1] + c[i];
            a[i+1] = b[i] + c[i+1];
            a[i+2] = b[i+1] + c[i+2];
            a[i+3] = b[i+2] + c[i+3];
        }
        // Scalar tail for second loop
        for (; i < loop_bound; i++) {
            a[i] = b[i-1] + c[i];
        }
    }
}
