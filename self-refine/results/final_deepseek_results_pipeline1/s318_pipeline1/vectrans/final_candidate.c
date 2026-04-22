#include <math.h>
#include <string.h>

float s318_opt(int iterations, int LEN_1D, float* a, int inc)
{
    int k, index;
    float max, chksum;
    
    for (int nl = 0; nl < iterations/2; nl++) {
        // Phase 1: Extract absolute values into temporary array
        float* abs_vals = (float*)malloc(LEN_1D * sizeof(float));
        k = 0;
        abs_vals[0] = fabsf(a[0]);
        k += inc;
        for (int i = 1; i < LEN_1D; i++) {
            abs_vals[i] = fabsf(a[k]);
            k += inc;
        }
        
        // Phase 2: Vectorizable max-finding on independent array
        max = abs_vals[0];
        index = 0;
        for (int i = 1; i < LEN_1D; i++) {
            float val = abs_vals[i];
            int cond = val > max;
            max = cond * val + (!cond) * max;
            index = cond * i + (!cond) * index;
        }
        
        free(abs_vals);
        chksum = max + (float) index;
    }
    return max + index + 1;
}
