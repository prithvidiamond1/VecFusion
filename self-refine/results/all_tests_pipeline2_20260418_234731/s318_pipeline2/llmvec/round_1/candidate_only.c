#include <stdint.h>
#include <string.h>
#include <math.h>

float vectorized_s318(int iterations, int LEN_1D, float* a, int inc) {
    int k, index;
    float max, chksum;
    
    for (int nl = 0; nl < iterations/2; nl++) {
        k = 0;
        index = 0;
        max = fabsf(a[0]);
        k += inc;

        // Precompute all absolute values
        float* abs_vals = (float*)__builtin_alloca(LEN_1D * sizeof(float));
        int current_k = k;
        for (int i = 1; i < LEN_1D; i++) {
            abs_vals[i] = fabsf(a[current_k]);
            current_k += inc;
        }

        // Vectorized maximum reduction
        float max_val = max;
        int i = 1;
        
        // Process in chunks of 4 for better vectorization potential
        for (; i + 3 < LEN_1D; i += 4) {
            float v0 = abs_vals[i];
            float v1 = abs_vals[i + 1];
            float v2 = abs_vals[i + 2];
            float v3 = abs_vals[i + 3];
            
            // Compare and update max
            if (v0 > max_val) max_val = v0;
            if (v1 > max_val) max_val = v1;
            if (v2 > max_val) max_val = v2;
            if (v3 > max_val) max_val = v3;
        }
        
        // Scalar tail
        for (; i < LEN_1D; i++) {
            float abs_val = abs_vals[i];
            if (abs_val > max_val) {
                max_val = abs_val;
            }
        }

        // Find first index where value equals maximum
        int max_idx = 0;
        for (i = 1; i < LEN_1D; i++) {
            if (abs_vals[i] == max_val) {
                max_idx = i;
                break;
            }
        }

        // Update original variables
        index = max_idx;
        max = max_val;
        chksum = max + (float) index;
    }
    return max + index + 1;
}
