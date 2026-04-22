#include <stdlib.h>
#include <string.h>

void vectorized_s2251(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations; nl++) {
        float s = (float)0.0;
        
        // Allocate temporary array
        float* s_vals = (float*)malloc(LEN_1D * sizeof(float));
        
        // First loop: compute s_vals[i] = b[i] + c[i]
        int i = 0;
        for (; i + 3 < LEN_1D; i += 4) {
            s_vals[i] = b[i] + c[i];
            s_vals[i+1] = b[i+1] + c[i+1];
            s_vals[i+2] = b[i+2] + c[i+2];
            s_vals[i+3] = b[i+3] + c[i+3];
        }
        for (; i < LEN_1D; i++) {
            s_vals[i] = b[i] + c[i];
        }
        
        // Second loop: compute a[i] = s_vals[i-1] * e[i] for i>=1
        a[0] = s * e[0];
        i = 1;
        for (; i + 3 < LEN_1D; i += 4) {
            a[i] = s_vals[i-1] * e[i];
            a[i+1] = s_vals[i] * e[i+1];
            a[i+2] = s_vals[i+1] * e[i+2];
            a[i+3] = s_vals[i+2] * e[i+3];
        }
        for (; i < LEN_1D; i++) {
            a[i] = s_vals[i-1] * e[i];
        }
        s = s_vals[LEN_1D-1];
        
        // Third loop: compute b[i] = a[i] + d[i]
        i = 0;
        for (; i + 3 < LEN_1D; i += 4) {
            b[i] = a[i] + d[i];
            b[i+1] = a[i+1] + d[i+1];
            b[i+2] = a[i+2] + d[i+2];
            b[i+3] = a[i+3] + d[i+3];
        }
        for (; i < LEN_1D; i++) {
            b[i] = a[i] + d[i];
        }
        
        free(s_vals);
    }
}
