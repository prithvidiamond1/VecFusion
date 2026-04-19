#include <stdint.h>
#include <string.h>

void vectorized_s342(int iterations, int LEN_1D, float* a, float *b) {
    for (int nl = 0; nl < iterations; nl++) {
        int prefix[LEN_1D];
        int count = -1;
        
        // First loop: compute prefix sum of conditions
        for (int i = 0; i < LEN_1D; i++) {
            int cond = (a[i] > 0.0f);
            count += cond;
            prefix[i] = cond ? count : -1;
        }
        
        // Second loop: vectorized assignment
        int i = 0;
        #if defined(__clang__)
        typedef float float4 __attribute__((ext_vector_type(4)));
        typedef int int4 __attribute__((ext_vector_type(4)));
        
        for (; i + 3 < LEN_1D; i += 4) {
            // Load indices
            int idx0 = prefix[i];
            int idx1 = prefix[i + 1];
            int idx2 = prefix[i + 2];
            int idx3 = prefix[i + 3];
            
            // Check which indices are valid
            int4 valid_mask = (int4){idx0 != -1, idx1 != -1, idx2 != -1, idx3 != -1};
            
            // Gather values from b using valid indices
            float4 gathered;
            gathered.x = (valid_mask.x) ? b[idx0] : a[i];
            gathered.y = (valid_mask.y) ? b[idx1] : a[i + 1];
            gathered.z = (valid_mask.z) ? b[idx2] : a[i + 2];
            gathered.w = (valid_mask.w) ? b[idx3] : a[i + 3];
            
            // Store back to a
            a[i] = gathered.x;
            a[i + 1] = gathered.y;
            a[i + 2] = gathered.z;
            a[i + 3] = gathered.w;
        }
        #endif
        
        // Scalar cleanup
        for (; i < LEN_1D; i++) {
            int idx = prefix[i];
            if (idx != -1) {
                a[i] = b[idx];
            }
        }
    }
}
