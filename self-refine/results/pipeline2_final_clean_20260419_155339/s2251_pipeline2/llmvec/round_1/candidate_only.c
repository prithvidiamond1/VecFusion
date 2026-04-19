#include <stddef.h>

void vectorized_s2251(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations; nl++) {
        float s = (float)0.0;
        float s_next;
        
        int i = 0;
        // Process elements one by one to handle the scalar dependency on s
        for (; i < LEN_1D; i++) {
            s_next = b[i] + c[i];
            a[i] = s * e[i];
            b[i] = a[i] + d[i];
            s = s_next;
        }
    }
}
