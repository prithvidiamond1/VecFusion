#include <stddef.h>

void vectorized_s1244(int iterations, int LEN_1D, float* a, float* b, float* c, float* d) {
    if (LEN_1D <= 1) return;
    
    int n = LEN_1D - 1;
    
    for (int nl = 0; nl < iterations; nl++) {
        int i = 0;
        
        // Vectorized main loop
        for (; i + 3 < n; i += 4) {
            // Load b[i..i+3]
            float b0 = b[i];
            float b1 = b[i+1];
            float b2 = b[i+2];
            float b3 = b[i+3];
            
            // Load c[i..i+3]
            float c0 = c[i];
            float c1 = c[i+1];
            float c2 = c[i+2];
            float c3 = c[i+3];
            
            // Load original a[i+1..i+4]
            float a_next0 = a[i+1];
            float a_next1 = a[i+2];
            float a_next2 = a[i+3];
            float a_next3 = a[i+4];
            
            // Compute new a[i] = b[i] + c[i]*c[i] + b[i]*b[i] + c[i]
            float a0 = b0 + c0*c0 + b0*b0 + c0;
            float a1 = b1 + c1*c1 + b1*b1 + c1;
            float a2 = b2 + c2*c2 + b2*b2 + c2;
            float a3 = b3 + c3*c3 + b3*b3 + c3;
            
            // Store new a[i..i+3]
            a[i] = a0;
            a[i+1] = a1;
            a[i+2] = a2;
            a[i+3] = a3;
            
            // Compute d[i] = new a[i] + original a[i+1]
            d[i] = a0 + a_next0;
            d[i+1] = a1 + a_next1;
            d[i+2] = a2 + a_next2;
            d[i+3] = a3 + a_next3;
        }
        
        // Scalar cleanup for remaining elements
        for (; i < n; i++) {
            float bi = b[i];
            float ci = c[i];
            float a_next = a[i+1];
            a[i] = bi + ci * ci + bi * bi + ci;
            d[i] = a[i] + a_next;
        }
    }
}
