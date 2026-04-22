#include <math.h>

float vectorized_s318(int iterations, int LEN_1D, float* a, int inc)
{
    int k, index;
    float max, chksum;
    for (int nl = 0; nl < iterations/2; nl++) {
        k = 0;
        index = 0;
        max = fabsf(a[0]);
        k += inc;

        // Process in chunks of 4 (unrolled for vectorization hint)
        int i = 1;
        // Scalar loop - but structured to be vectorization-friendly
        // Since we have a data-dependent index tracking (find max abs),
        // we can vectorize by tracking max and index in parallel chunks
        
        // Use a simple unrolled approach tracking 4 candidates at once
        float max0 = max, max1 = -1.0f, max2 = -1.0f, max3 = -1.0f;
        int idx0 = index, idx1 = 0, idx2 = 0, idx3 = 0;
        
        // Reset and redo properly
        max = fabsf(a[0]);
        index = 0;
        k = inc; // k = 1*inc after first element
        
        int chunk = (LEN_1D - 1) / 4;
        int rem_start = 1 + chunk * 4;
        
        // Process 4 elements at a time
        int ki = k;
        for (int ci = 0; ci < chunk; ci++) {
            float v0 = fabsf(a[ki]);          ki += inc;
            float v1 = fabsf(a[ki]);          ki += inc;
            float v2 = fabsf(a[ki]);          ki += inc;
            float v3 = fabsf(a[ki]);          ki += inc;
            
            int base = 1 + ci * 4;
            
            if (v0 > max) { max = v0; index = base + 0; }
            if (v1 > max) { max = v1; index = base + 1; }
            if (v2 > max) { max = v2; index = base + 2; }
            if (v3 > max) { max = v3; index = base + 3; }
        }
        
        // Scalar cleanup
        k = ki;
        for (int i2 = rem_start; i2 < LEN_1D; i2++) {
            float v = fabsf(a[k]);
            if (v > max) {
                max = v;
                index = i2;
            }
            k += inc;
        }
        
        chksum = max + (float)index;
    }
    return max + index + 1;
}
