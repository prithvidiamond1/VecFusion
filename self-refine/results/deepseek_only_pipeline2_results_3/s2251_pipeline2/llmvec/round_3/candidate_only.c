#include <stdlib.h>

typedef float v8f32 __attribute__((__vector_size__(32), __aligned__(32)));

void vectorized_s2251(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    const int VEC_SIZE = 8;
    
    for (int nl = 0; nl < iterations; nl++) {
        float s = (float)0.0;
        
        // First iteration (scalar)
        a[0] = s * e[0];
        b[0] = a[0] + d[0];
        s = b[0] + c[0];
        
        // Main loop - process with scalar recurrence
        int i = 1;
        for (; i + VEC_SIZE <= LEN_1D; i += VEC_SIZE) {
            // Process vector chunk with explicit scalar recurrence
            for (int j = 0; j < VEC_SIZE; j++) {
                int idx = i + j;
                a[idx] = s * e[idx];
                b[idx] = a[idx] + d[idx];
                s = b[idx] + c[idx];
            }
        }
        
        // Scalar tail for remaining elements
        for (; i < LEN_1D; i++) {
            a[i] = s * e[i];
            b[i] = a[i] + d[i];
            s = b[i] + c[i];
        }
    }
}
