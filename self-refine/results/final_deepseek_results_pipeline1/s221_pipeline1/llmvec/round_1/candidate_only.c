```c
#include <stddef.h>

typedef float float_vec __attribute__((__vector_size__(16)));

void vectorized_s221(int iterations, int LEN_1D, float *a, float *b, float *c, float *d) {
    int total_iters = iterations / 2;
    for (int nl = 0; nl < total_iters; nl++) {
        int i = 1;
        // Handle the first element scalar to set up b[1] for vectorization
        a[1] += c[1] * d[1];
        b[1] = b[0] + a[1] + d[1];
        i = 2;
        
        // Vectorized main loop: process 4 elements at a time
        for (; i + 3 < LEN_1D; i += 4) {
            float_vec va = *(float_vec *)&a[i];
            float_vec vc = *(float_vec *)&c[i];
            float_vec vd = *(float_vec *)&d[i];
            float_vec vb_prev = *(float_vec *)&b[i - 1];
            
            // a[i..i+3] += c[i..i+3] * d[i..i+3]
            va = va + vc * vd;
            *(float_vec *)&a[i] = va;
            
            // b[i] = b[i-1] + a[i] + d[i]
            // b[i+1] = b[i] + a[i+1] + d[i+1]
            // b[i+2] = b[i+1] + a[i+2] + d[i+2]
            // b[i+3] = b[i+2] + a[i+3] + d[i+3]
            // This is a prefix sum, so we compute sequentially
            float temp_b = vb_prev[3] + va[0] + vd[0];
            b[i] = temp_b;
            temp_b = temp_b + va[1] + vd[1];
            b[i + 1] = temp_b;
            temp_b = temp_b + va[2] + vd[2];
            b[i + 2] = temp_b;
            temp_b = temp_b + va[3] + vd[3];
            b[i + 3] = temp_b;
        }
        
        // Scalar cleanup for remaining elements
        for (; i < LEN_1D; i++) {
            a[i] += c[i] * d[i];
            b[i] = b[i - 1] + a[i] + d[i];
        }
    }
}
