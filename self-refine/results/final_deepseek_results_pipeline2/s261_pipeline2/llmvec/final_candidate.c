#include <stddef.h>

void vectorized_s261(int iterations, int LEN_1D, float* a, float* b, float* c, float* d) {
    if (LEN_1D <= 1 || iterations <= 0) return;
    
    for (int nl = 0; nl < iterations; nl++) {
        // Vectorize c[i] = c[i] * d[i] for i >= 1
        int i = 1;
        int end = LEN_1D;
        
        // Main vectorized loop for c update
        for (; i + 3 < end; i += 4) {
            c[i] = c[i] * d[i];
            c[i+1] = c[i+1] * d[i+1];
            c[i+2] = c[i+2] * d[i+2];
            c[i+3] = c[i+3] * d[i+3];
        }
        // Scalar tail for c update
        for (; i < end; i++) {
            c[i] = c[i] * d[i];
        }
        
        // Vectorize a[i] = (a[i] + b[i]) + c[i-1] for i >= 1
        i = 1;
        // Main vectorized loop for a update
        for (; i + 3 < end; i += 4) {
            a[i] = (a[i] + b[i]) + c[i-1];
            a[i+1] = (a[i+1] + b[i+1]) + c[i];
            a[i+2] = (a[i+2] + b[i+2]) + c[i+1];
            a[i+3] = (a[i+3] + b[i+3]) + c[i+2];
        }
        // Scalar tail for a update
        for (; i < end; i++) {
            a[i] = (a[i] + b[i]) + c[i-1];
        }
    }
}
