#include <stdint.h>

void vectorized_s292(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Handle first two iterations separately to break dependency chain
        a[0] = (b[0] + b[LEN_1D-1] + b[LEN_1D-2]) * (float).333;
        a[1] = (b[1] + b[0] + b[LEN_1D-1]) * (float).333;

        // Main vectorizable loop for i >= 2
        // Unroll by 4 for SIMD-friendly code
        int i = 2;
        int limit = LEN_1D - 4;
        for (; i <= limit; i += 4) {
            a[i+0] = (b[i+0] + b[i-1] + b[i-2]) * (float).333;
            a[i+1] = (b[i+1] + b[i+0] + b[i-1]) * (float).333;
            a[i+2] = (b[i+2] + b[i+1] + b[i+0]) * (float).333;
            a[i+3] = (b[i+3] + b[i+2] + b[i+1]) * (float).333;
        }
        // Scalar cleanup
        for (; i < LEN_1D; i++) {
            a[i] = (b[i] + b[i-1] + b[i-2]) * (float).333;
        }
    }
}
