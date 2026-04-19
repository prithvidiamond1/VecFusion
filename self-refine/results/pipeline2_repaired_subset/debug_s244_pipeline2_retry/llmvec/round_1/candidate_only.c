#include <stddef.h>

void vectorized_s244(int iterations, int LEN_1D, float* a, float* b, float* c, float* d) {
    for (int nl = 0; nl < iterations; nl++) {
        // First loop: vectorized computation of a[i] and b[i]
        int i = 0;
        for (; i <= LEN_1D - 5; i += 4) {
            // Load vectors
            float b_vec[4] = {b[i], b[i+1], b[i+2], b[i+3]};
            float c_vec[4] = {c[i], c[i+1], c[i+2], c[i+3]};
            float d_vec[4] = {d[i], d[i+1], d[i+2], d[i+3]};
            
            // Compute a[i] = b[i] + c[i] * d[i]
            float a_vec[4];
            for (int j = 0; j < 4; j++) {
                a_vec[j] = b_vec[j] + c_vec[j] * d_vec[j];
            }
            
            // Store a[i]
            for (int j = 0; j < 4; j++) {
                a[i+j] = a_vec[j];
            }
            
            // Compute b[i] = c[i] + b[i]
            for (int j = 0; j < 4; j++) {
                b_vec[j] = c_vec[j] + b_vec[j];
            }
            
            // Store updated b[i]
            for (int j = 0; j < 4; j++) {
                b[i+j] = b_vec[j];
            }
        }
        
        // Scalar cleanup for first loop
        for (; i < LEN_1D - 1; i++) {
            a[i] = b[i] + c[i] * d[i];
            b[i] = c[i] + b[i];
        }
        
        // Second loop: vectorized computation of a[i+1]
        i = 0;
        for (; i <= LEN_1D - 5; i += 4) {
            // Load b[i] (already updated from first loop)
            float b_vec[4] = {b[i], b[i+1], b[i+2], b[i+3]};
            
            // Load a[i+1] (original values)
            float a_next_vec[4] = {a[i+1], a[i+2], a[i+3], a[i+4]};
            
            // Load d[i]
            float d_vec[4] = {d[i], d[i+1], d[i+2], d[i+3]};
            
            // Compute a[i+1] = b[i] + a[i+1] * d[i]
            for (int j = 0; j < 4; j++) {
                a_next_vec[j] = b_vec[j] + a_next_vec[j] * d_vec[j];
            }
            
            // Store updated a[i+1]
            for (int j = 0; j < 4; j++) {
                a[i+1+j] = a_next_vec[j];
            }
        }
        
        // Scalar cleanup for second loop
        for (; i < LEN_1D - 1; i++) {
            float temp = a[i+1];
            a[i+1] = b[i] + temp * d[i];
        }
    }
}
