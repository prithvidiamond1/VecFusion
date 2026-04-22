#include <stddef.h>

typedef float float32_t __attribute__((__vector_size__(16)));

void vectorized_s1213(int iterations, int LEN_1D, float* a, float* b, float* c, float* d) {
    if (LEN_1D < 3) return;
    
    for (int nl = 0; nl < iterations; nl++) {
        int i = 1;
        // Vectorize 4 elements at a time, ensuring we don't read past array bounds
        // Need a[i+1]..a[i+4] and b[i-1]..b[i+2], so max i is LEN_1D-5
        for (; i <= LEN_1D - 5; i += 4) {
            // Load b[i-1..i+2] (before b[i] is updated)
            float32_t b_orig = *(float32_t*)(b + i - 1);
            
            // Load c[i..i+3]
            float32_t c_vec = *(float32_t*)(c + i);
            
            // Compute a[i..i+3] = b[i-1..i+2] + c[i..i+3]
            float32_t a_new = b_orig + c_vec;
            *(float32_t*)(a + i) = a_new;
            
            // Load a[i+1..i+4] (after a[i..i+3] is written, but a[i+1..i+4] is safe)
            float32_t a_next = *(float32_t*)(a + i + 1);
            
            // Load d[i..i+3]
            float32_t d_vec = *(float32_t*)(d + i);
            
            // Compute b[i..i+3] = a[i+1..i+4] * d[i..i+3]
            float32_t b_new = a_next * d_vec;
            *(float32_t*)(b + i) = b_new;
        }
        
        // Scalar cleanup for remaining elements
        for (; i < LEN_1D - 1; i++) {
            a[i] = b[i-1] + c[i];
            b[i] = a[i+1] * d[i];
        }
    }
}
