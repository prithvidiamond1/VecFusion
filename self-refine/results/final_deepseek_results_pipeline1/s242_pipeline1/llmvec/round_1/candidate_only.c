#include <stddef.h>

typedef float float_vec __attribute__((__vector_size__(16)));

void vectorized_s242(int iterations, int LEN_1D, float s1, float s2, float* a, float* b, float* c, float* d) {
    int total_iters = iterations / 5;
    if (total_iters <= 0 || LEN_1D <= 1) return;
    
    float sum = s1 + s2;
    
    for (int nl = 0; nl < total_iters; nl++) {
        int i = 1;
        
        // Vectorized main loop: process 4 elements at a time
        for (; i + 3 < LEN_1D; i += 4) {
            float_vec vb = *(float_vec*)(b + i);
            float_vec vc = *(float_vec*)(c + i);
            float_vec vd = *(float_vec*)(d + i);
            float_vec va_prev = *(float_vec*)(a + i - 1);
            
            float_vec vsum = va_prev + (float_vec){sum, sum, sum, sum} + vb + vc + vd;
            
            // Handle the recurrence: a[i] depends on a[i-1], a[i+1] depends on a[i], etc.
            // We need to compute sequentially within the vector
            float temp[4];
            temp[0] = a[i-1] + sum + b[i] + c[i] + d[i];
            a[i] = temp[0];
            temp[1] = temp[0] + sum + b[i+1] + c[i+1] + d[i+1];
            a[i+1] = temp[1];
            temp[2] = temp[1] + sum + b[i+2] + c[i+2] + d[i+2];
            a[i+2] = temp[2];
            temp[3] = temp[2] + sum + b[i+3] + c[i+3] + d[i+3];
            a[i+3] = temp[3];
        }
        
        // Scalar cleanup for remaining elements
        for (; i < LEN_1D; ++i) {
            a[i] = a[i - 1] + sum + b[i] + c[i] + d[i];
        }
    }
}
