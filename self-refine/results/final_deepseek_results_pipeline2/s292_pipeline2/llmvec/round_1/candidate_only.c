#include <stdint.h>

void vectorized_s292(int iterations, int LEN_1D, float* a, float* b) {
    if (LEN_1D < 3) return;
    
    for (int nl = 0; nl < iterations; nl++) {
        // Handle first two elements with wrap-around separately
        a[0] = (b[0] + b[LEN_1D-1] + b[LEN_1D-2]) * (float).333;
        a[1] = (b[1] + b[0] + b[LEN_1D-1]) * (float).333;
        
        // Main vectorizable loop for i=2 to LEN_1D-1
        int i = 2;
        // Vectorized section
        for (; i + 3 < LEN_1D; i += 4) {
            a[i] = (b[i] + b[i-1] + b[i-2]) * (float).333;
            a[i+1] = (b[i+1] + b[i] + b[i-1]) * (float).333;
            a[i+2] = (b[i+2] + b[i+1] + b[i]) * (float).333;
            a[i+3] = (b[i+3] + b[i+2] + b[i+1]) * (float).333;
        }
        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            a[i] = (b[i] + b[i-1] + b[i-2]) * (float).333;
        }
    }
}
