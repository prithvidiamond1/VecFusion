#include <stddef.h>

float vectorized_s3112(int iterations, int LEN_1D, float* a, float* b)
{
    float sum = 0.0f;
    
    for (int nl = 0; nl < iterations; nl++) {
        // Vectorized reduction
        float temp_sum = 0.0f;
        int i = 0;
        
        // Process in chunks of 4 for better vectorization potential
        for (; i + 3 < LEN_1D; i += 4) {
            temp_sum += a[i] + a[i+1] + a[i+2] + a[i+3];
        }
        
        // Scalar tail
        for (; i < LEN_1D; i++) {
            temp_sum += a[i];
        }
        
        sum = temp_sum;
        
        // Prefix sum - use explicit dependency breaking
        float prefix_sum = 0.0f;
        i = 0;
        
        // Process in chunks of 4 with explicit unrolling
        for (; i + 3 < LEN_1D; i += 4) {
            float a0 = a[i];
            float a1 = a[i+1];
            float a2 = a[i+2];
            float a3 = a[i+3];
            
            prefix_sum += a0;
            b[i] = prefix_sum;
            
            prefix_sum += a1;
            b[i+1] = prefix_sum;
            
            prefix_sum += a2;
            b[i+2] = prefix_sum;
            
            prefix_sum += a3;
            b[i+3] = prefix_sum;
        }
        
        // Scalar tail for prefix sum
        for (; i < LEN_1D; i++) {
            float current_a = a[i];
            prefix_sum += current_a;
            b[i] = prefix_sum;
        }
    }
    
    return sum;
}
