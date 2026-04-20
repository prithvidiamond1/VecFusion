#include <stdint.h>
#include <string.h>

float vectorized_s352(int iterations, int LEN_1D, float* a, float *b)
{
    float dot;
    for (int nl = 0; nl < 8*iterations; nl++) {
        dot = 0.0f;
        
        // Process groups of 5 elements using vector operations
        int i;
        for (i = 0; i + 4 < LEN_1D; i += 5) {
            // Load 5 elements from a and b
            float a0 = a[i];
            float a1 = a[i+1];
            float a2 = a[i+2];
            float a3 = a[i+3];
            float a4 = a[i+4];
            
            float b0 = b[i];
            float b1 = b[i+1];
            float b2 = b[i+2];
            float b3 = b[i+3];
            float b4 = b[i+4];
            
            // Multiply and accumulate
            dot += a0 * b0 + a1 * b1 + a2 * b2 + a3 * b3 + a4 * b4;
        }
        
        // Scalar cleanup for remaining elements
        for (; i < LEN_1D; i++) {
            dot += a[i] * b[i];
        }
    }
    return dot;
}
