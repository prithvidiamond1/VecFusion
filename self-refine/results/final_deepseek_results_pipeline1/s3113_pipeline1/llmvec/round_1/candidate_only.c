#include <stddef.h>
#include <math.h>

typedef float float32_t;

float vectorized_s3113(int iterations, int LEN_1D, float* a) {
    float max = 0.0f;
    int total_iters = iterations * 4;
    
    for (int nl = 0; nl < total_iters; nl++) {
        float current_max = fabsf(a[0]);
        int i = 1;
        
        // Vectorized main loop (process 4 elements at a time)
        for (; i <= LEN_1D - 4; i += 4) {
            float v0 = fabsf(a[i]);
            float v1 = fabsf(a[i+1]);
            float v2 = fabsf(a[i+2]);
            float v3 = fabsf(a[i+3]);
            
            // Find max among the 4 values
            float m01 = (v0 > v1) ? v0 : v1;
            float m23 = (v2 > v3) ? v2 : v3;
            float m = (m01 > m23) ? m01 : m23;
            
            if (m > current_max) {
                current_max = m;
            }
        }
        
        // Scalar cleanup for remaining elements
        for (; i < LEN_1D; i++) {
            float val = fabsf(a[i]);
            if (val > current_max) {
                current_max = val;
            }
        }
        
        max = current_max;
    }
    
    return max;
}
