#include <stdint.h>

float vectorized_s352(int iterations, int LEN_1D, float* a, float *b)
{
    float dot;
    for (int nl = 0; nl < iterations; nl++) {
        dot = 0.0f;
        
        // Process groups of 5 elements with explicit sequential accumulation
        int i;
        for (i = 0; i + 4 < LEN_1D; i += 5) {
            // Load and multiply elements in original order
            float t0 = a[i] * b[i];
            float t1 = a[i+1] * b[i+1];
            float t2 = a[i+2] * b[i+2];
            float t3 = a[i+3] * b[i+3];
            float t4 = a[i+4] * b[i+4];
            
            // Accumulate in the exact same order as scalar version
            dot += t0;
            dot += t1;
            dot += t2;
            dot += t3;
            dot += t4;
        }
        
        // Scalar cleanup for remaining elements
        for (; i < LEN_1D; i++) {
            dot += a[i] * b[i];
        }
    }
    return dot;
}
