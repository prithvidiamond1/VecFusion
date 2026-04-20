#include <stddef.h>

void vectorized_s242(int iterations, int LEN_1D, float s1, float s2, float* a, float* b, float* c, float* d) {
    float sum_s = s1 + s2;
    float prev;
    
    int outer_limit = iterations / 5;
    
    for (int nl = 0; nl < outer_limit; nl++) {
        prev = a[0];
        
        // Vectorizable independent computation
        int i = 1;
        // Process in chunks for better vectorization potential
        for (; i + 3 < LEN_1D; i += 4) {
            a[i] = sum_s + b[i] + c[i] + d[i];
            a[i + 1] = sum_s + b[i + 1] + c[i + 1] + d[i + 1];
            a[i + 2] = sum_s + b[i + 2] + c[i + 2] + d[i + 2];
            a[i + 3] = sum_s + b[i + 3] + c[i + 3] + d[i + 3];
        }
        // Scalar tail
        for (; i < LEN_1D; i++) {
            a[i] = sum_s + b[i] + c[i] + d[i];
        }
        
        // Sequential recurrence - cannot be vectorized due to dependency
        for (int i = 1; i < LEN_1D; i++) {
            prev = prev + a[i];
            a[i] = prev;
        }
    }
}
