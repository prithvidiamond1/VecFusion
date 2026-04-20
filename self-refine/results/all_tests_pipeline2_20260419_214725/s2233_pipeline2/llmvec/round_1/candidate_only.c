#include <stdint.h>

void vectorized_s2233(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    int outer_loop_count = 100 * (iterations / 256);
    
    for (int nl = 0; nl < outer_loop_count; nl++) {
        // Process aa and bb updates in the original order
        for (int i = 1; i < 256; i++) {
            // aa update: dependency on aa[j-1][i] prevents vectorization in j
            // Keep as scalar loop for semantic correctness
            for (int j = 1; j < 256; j++) {
                aa[j][i] = aa[j-1][i] + cc[j][i];
            }

            // bb update: dependency on bb[i-1][j] is loop-invariant for inner j loop
            // Vectorize in j dimension using 4-element SIMD
            int j;
            for (j = 1; j <= 256 - 4; j += 4) {
                // Load bb[i-1][j:j+3]
                float bb_prev_0 = bb[i-1][j];
                float bb_prev_1 = bb[i-1][j+1];
                float bb_prev_2 = bb[i-1][j+2];
                float bb_prev_3 = bb[i-1][j+3];
                
                // Load cc[i][j:j+3]
                float cc_0 = cc[i][j];
                float cc_1 = cc[i][j+1];
                float cc_2 = cc[i][j+2];
                float cc_3 = cc[i][j+3];
                
                // Compute bb[i][j:j+3] = bb[i-1][j:j+3] + cc[i][j:j+3]
                bb[i][j]   = bb_prev_0 + cc_0;
                bb[i][j+1] = bb_prev_1 + cc_1;
                bb[i][j+2] = bb_prev_2 + cc_2;
                bb[i][j+3] = bb_prev_3 + cc_3;
            }
            
            // Scalar cleanup for remaining elements
            for (; j < 256; j++) {
                bb[i][j] = bb[i-1][j] + cc[i][j];
            }
        }
    }
}
