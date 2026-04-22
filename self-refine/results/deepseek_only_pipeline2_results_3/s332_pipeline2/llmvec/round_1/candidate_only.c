#include <stdint.h>
#include <string.h>

float vectorized_s332(int iterations, int LEN_1D, int t, float* a) {
    float value = 0.0f;
    
    for (int nl = 0; nl < iterations; nl++) {
        int index = -2;
        value = -1.0f;
        
        // Vectorized search for first element > t
        int found_index = -2;
        int i = 0;
        
        // Process in chunks for potential vectorization
        for (; i + 3 < LEN_1D; i += 4) {
            // Check 4 elements at once
            int match0 = a[i] > t;
            int match1 = a[i + 1] > t;
            int match2 = a[i + 2] > t;
            int match3 = a[i + 3] > t;
            
            // Find first match in this chunk
            if (found_index == -2) {
                if (match0) {
                    found_index = i;
                } else if (match1) {
                    found_index = i + 1;
                } else if (match2) {
                    found_index = i + 2;
                } else if (match3) {
                    found_index = i + 3;
                }
            }
        }
        
        // Handle remaining elements
        for (; i < LEN_1D; i++) {
            if (found_index == -2 && a[i] > t) {
                found_index = i;
            }
        }
        
        // Extract value if index was found
        if (found_index != -2) {
            index = found_index;
            value = a[found_index];
        }
        
        // Final computation (kept for semantic correctness)
        float chksum = value + (float)index;
        (void)chksum; // Prevent unused variable warning
    }
    
    return value;
}
