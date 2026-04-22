#include <stdint.h>

void vectorized_s442(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e, int* indx) {
    int loop_count = iterations / 2;
    
    for (int nl = 0; nl < loop_count; nl++) {
        // Vectorize each case separately with explicit bounds
        int i = 0;
        
        // Case 1: indx[i] == 1
        for (i = 0; i < LEN_1D; i++) {
            if (indx[i] == 1) {
                a[i] += b[i] * b[i];
            }
        }
        
        // Case 2: indx[i] == 2
        for (i = 0; i < LEN_1D; i++) {
            if (indx[i] == 2) {
                a[i] += c[i] * c[i];
            }
        }
        
        // Case 3: indx[i] == 3
        for (i = 0; i < LEN_1D; i++) {
            if (indx[i] == 3) {
                a[i] += d[i] * d[i];
            }
        }
        
        // Case 4: indx[i] == 4
        for (i = 0; i < LEN_1D; i++) {
            if (indx[i] == 4) {
                a[i] += e[i] * e[i];
            }
        }
    }
}
