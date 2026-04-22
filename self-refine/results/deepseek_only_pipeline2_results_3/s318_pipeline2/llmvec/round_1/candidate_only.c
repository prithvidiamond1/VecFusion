#include <math.h>
#include <stdint.h>

float vectorized_s318(int iterations, int LEN_1D, float* a, int inc) {
    int k, index;
    float max, chksum;
    
    for (int nl = 0; nl < iterations/2; nl++) {
        k = 0;
        index = 0;
        max = fabsf(a[0]);
        
        // First pass: find maximum absolute value (reduction)
        float max_val = max;
        
        // Vectorized reduction for maximum absolute value
        int i = 1;
        typedef float v4f __attribute__((vector_size(16)));
        v4f vmax = {max_val, max_val, max_val, max_val};
        
        // Process in chunks of 4
        for (; i + 3 < LEN_1D; i += 4) {
            k += inc;
            float abs_val0 = fabsf(a[k]);
            k += inc;
            float abs_val1 = fabsf(a[k]);
            k += inc;
            float abs_val2 = fabsf(a[k]);
            k += inc;
            float abs_val3 = fabsf(a[k]);
            
            v4f vabs = {abs_val0, abs_val1, abs_val2, abs_val3};
            v4f vcmp = vabs > vmax;
            
            // Blend operation: vmax = vcmp ? vabs : vmax
            vmax = (vabs & vcmp) | (vmax & ~vcmp);
        }
        
        // Extract horizontal maximum from vector
        float vmax_arr[4];
        __builtin_memcpy(vmax_arr, &vmax, sizeof(vmax));
        for (int j = 0; j < 4; j++) {
            if (vmax_arr[j] > max_val) {
                max_val = vmax_arr[j];
            }
        }
        
        // Scalar tail for first pass
        for (; i < LEN_1D; i++) {
            k += inc;
            float abs_val = fabsf(a[k]);
            if (abs_val > max_val) {
                max_val = abs_val;
            }
        }
        
        // Second pass: find first index where value equals max_val
        k = 0;
        int found_idx = 0;
        
        // Vectorized search for first occurrence
        i = 0;
        int found = 0;
        
        for (; i + 3 < LEN_1D && !found; i += 4) {
            float abs_val0 = fabsf(a[k]);
            k += inc;
            float abs_val1 = fabsf(a[k]);
            k += inc;
            float abs_val2 = fabsf(a[k]);
            k += inc;
            float abs_val3 = fabsf(a[k]);
            k += inc;
            
            // Check each element in the chunk
            if (!found && abs_val0 == max_val) {
                found_idx = i;
                found = 1;
            }
            if (!found && abs_val1 == max_val) {
                found_idx = i + 1;
                found = 1;
            }
            if (!found && abs_val2 == max_val) {
                found_idx = i + 2;
                found = 1;
            }
            if (!found && abs_val3 == max_val) {
                found_idx = i + 3;
                found = 1;
            }
        }
        
        // Scalar tail for second pass
        for (; i < LEN_1D && !found; i++) {
            float abs_val = fabsf(a[k]);
            if (abs_val == max_val) {
                found_idx = i;
                found = 1;
            }
            k += inc;
        }
        
        max = max_val;
        index = found_idx;
        chksum = max + (float) index;
    }
    return max + index + 1;
}
