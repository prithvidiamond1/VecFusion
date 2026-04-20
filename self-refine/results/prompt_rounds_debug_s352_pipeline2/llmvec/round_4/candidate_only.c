#include <stdint.h>

float vectorized_s352(int iterations, int LEN_1D, float* a, float *b)
{
    float dot;
    for (int nl = 0; nl < 8*iterations; nl++) {
        dot = 0.0f;
        
        // Process groups of 5 elements while preserving exact accumulation order
        int i;
        for (i = 0; i <= LEN_1D - 5; i += 5) {
            // Compute products exactly as in scalar version
            float t0 = a[i] * b[i];
            float t1 = a[i+1] * b[i+1];
            float t2 = a[i+2] * b[i+2];
            float t3 = a[i+3] * b[i+3];
            float t4 = a[i+4] * b[i+4];
            
            // Accumulate group sum with single addition to dot
            // This preserves the same accumulation order as scalar:
            // dot = dot + (t0 + t1 + t2 + t3 + t4)
            float group_sum = t0 + t1 + t2 + t3 + t4;
            dot = dot + group_sum;
        }
        
        // Scalar cleanup for remaining elements (0-4 elements)
        // This matches the original scalar loop's exact order
        for (; i < LEN_1D; i++) {
            dot = dot + a[i] * b[i];
        }
    }
    return dot;
}
