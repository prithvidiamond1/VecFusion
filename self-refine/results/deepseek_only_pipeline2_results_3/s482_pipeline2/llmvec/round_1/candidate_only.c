#include <stddef.h>

void vectorized_s482(int iterations, int LEN_1D, float* a, float *b, float *c) {
    for (int nl = 0; nl < iterations; nl++) {
        int break_index = LEN_1D;
        
        // Find break point first (scalar search)
        for (int i = 0; i < LEN_1D; i++) {
            if (c[i] > b[i]) {
                break_index = i;
                break;
            }
        }
        
        // Vector-friendly main loop
        int i = 0;
        for (; i + 3 < break_index; i += 4) {
            a[i] += b[i] * c[i];
            a[i+1] += b[i+1] * c[i+1];
            a[i+2] += b[i+2] * c[i+2];
            a[i+3] += b[i+3] * c[i+3];
        }
        
        // Scalar tail
        for (; i < break_index; i++) {
            a[i] += b[i] * c[i];
        }
        
        // Handle the break index element separately
        if (break_index < LEN_1D) {
            a[break_index] += b[break_index] * c[break_index];
        }
    }
}
