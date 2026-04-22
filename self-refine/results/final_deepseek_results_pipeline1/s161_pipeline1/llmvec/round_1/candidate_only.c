#include <stdint.h>
#include <stdbool.h>

void vectorized_s161(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e) {
    int total_iters = iterations / 2;
    for (int nl = 0; nl < total_iters; nl++) {
        int i = 0;
        // Process in chunks of 4 for potential vectorization
        for (; i <= LEN_1D - 5; i += 4) {
            // Load b values
            float b0 = b[i];
            float b1 = b[i+1];
            float b2 = b[i+2];
            float b3 = b[i+3];
            
            // Check conditions and compute results
            bool cond0 = b0 < 0.0f;
            bool cond1 = b1 < 0.0f;
            bool cond2 = b2 < 0.0f;
            bool cond3 = b3 < 0.0f;
            
            // Compute both paths for each element
            float a_val0 = c[i] + d[i] * e[i];
            float a_val1 = c[i+1] + d[i+1] * e[i+1];
            float a_val2 = c[i+2] + d[i+2] * e[i+2];
            float a_val3 = c[i+3] + d[i+3] * e[i+3];
            
            float c_next0 = a[i] + d[i] * d[i];
            float c_next1 = a[i+1] + d[i+1] * d[i+1];
            float c_next2 = a[i+2] + d[i+2] * d[i+2];
            float c_next3 = a[i+3] + d[i+3] * d[i+3];
            
            // Select based on condition
            a[i] = cond0 ? a[i] : a_val0;
            a[i+1] = cond1 ? a[i+1] : a_val1;
            a[i+2] = cond2 ? a[i+2] : a_val2;
            a[i+3] = cond3 ? a[i+3] : a_val3;
            
            c[i+1] = cond0 ? c_next0 : c[i+1];
            c[i+2] = cond1 ? c_next1 : c[i+2];
            c[i+3] = cond2 ? c_next2 : c[i+3];
            c[i+4] = cond3 ? c_next3 : c[i+4];
        }
        // Scalar cleanup for remaining elements
        for (; i < LEN_1D - 1; ++i) {
            if (b[i] < 0.0f) {
                c[i+1] = a[i] + d[i] * d[i];
            } else {
                a[i] = c[i] + d[i] * e[i];
            }
        }
    }
}
