#include <stdlib.h>
#include <string.h>

void vectorized_s241(int iterations, int LEN_1D, float* a, float* b, float* c, float* d) {
    float* a_old = (float*)malloc(LEN_1D * sizeof(float));
    int loop_bound = LEN_1D - 1;
    
    for (int nl = 0; nl < 2 * iterations; nl++) {
        // Vector-friendly copy of a to a_old
        int i = 0;
        for (; i + 3 < LEN_1D; i += 4) {
            a_old[i] = a[i];
            a_old[i + 1] = a[i + 1];
            a_old[i + 2] = a[i + 2];
            a_old[i + 3] = a[i + 3];
        }
        for (; i < LEN_1D; i++) {
            a_old[i] = a[i];
        }
        
        // Vector-friendly computation of a[i] = b[i] * c[i] * d[i]
        i = 0;
        for (; i + 3 < loop_bound; i += 4) {
            a[i] = b[i] * c[i] * d[i];
            a[i + 1] = b[i + 1] * c[i + 1] * d[i + 1];
            a[i + 2] = b[i + 2] * c[i + 2] * d[i + 2];
            a[i + 3] = b[i + 3] * c[i + 3] * d[i + 3];
        }
        for (; i < loop_bound; i++) {
            a[i] = b[i] * c[i] * d[i];
        }
        
        // Vector-friendly computation of b[i] = a[i] * a_old[i+1] * d[i]
        i = 0;
        for (; i + 3 < loop_bound; i += 4) {
            b[i] = a[i] * a_old[i + 1] * d[i];
            b[i + 1] = a[i + 1] * a_old[i + 2] * d[i + 1];
            b[i + 2] = a[i + 2] * a_old[i + 3] * d[i + 2];
            b[i + 3] = a[i + 3] * a_old[i + 4] * d[i + 3];
        }
        for (; i < loop_bound; i++) {
            b[i] = a[i] * a_old[i + 1] * d[i];
        }
    }
    
    free(a_old);
}
