#include <stddef.h>

void vectorized_s141(int iterations, float * flat_2d_array, float bb[256][256]) {
    int nl_iter = 200 * (iterations / 256);
    
    for (int nl = 0; nl < nl_iter; nl++) {
        // Precompute k_start values for each row i
        int k_start[256];
        for (int i = 0; i < 256; i++) {
            k_start[i] = (i + 1) * i / 2 + i;
        }
        
        // Process by j first for regular access pattern
        for (int j = 0; j < 256; j++) {
            // Compute initial k for i=0
            int k = k_start[0] + j;
            
            // Process i from 0 to j inclusive
            for (int i = 0; i <= j; i++) {
                flat_2d_array[k] += bb[j][i];
                
                // Update k for next i if not at last iteration
                if (i < j) {
                    k += (j - i);
                }
            }
        }
    }
}
