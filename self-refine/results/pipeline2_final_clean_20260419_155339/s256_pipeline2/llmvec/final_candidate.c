#include <stdint.h>

void vectorized_s256(int iterations, float* a, float* d, float aa[256][256], float bb[256][256])
{
    int loop_count = 10 * (iterations / 256);
    
    for (int nl = 0; nl < loop_count; nl++) {
        float a0 = a[0];
        float a_temp[256];
        a_temp[0] = a0;
        
        // Vectorized computation of a_temp[j]
        for (int j = 1; j < 256; j += 4) {
            // Create pattern: [a0, 1-a0, a0, 1-a0] for consecutive j values
            float vec_a0[4] = {a0, a0, a0, a0};
            float vec_one_minus_a0[4] = {1.0f - a0, 1.0f - a0, 1.0f - a0, 1.0f - a0};
            
            // Pattern based on j & 1
            int pattern[4] = {1 & 1, (j+1) & 1, (j+2) & 1, (j+3) & 1};
            
            for (int k = 0; k < 4; k++) {
                a_temp[j + k] = pattern[k] ? vec_one_minus_a0[k] : vec_a0[k];
            }
        }
        
        // Update original a array
        for (int j = 1; j < 256; j++) {
            a[j] = a_temp[j];
        }
        
        // Vectorized computation of aa[j][i]
        for (int i = 0; i < 256; i++) {
            for (int j = 1; j < 256; j += 4) {
                // Process 4 j values at a time
                for (int k = 0; k < 4 && (j + k) < 256; k++) {
                    aa[j + k][i] = a_temp[j + k] + bb[j + k][i] * d[j + k];
                }
            }
        }
    }
}
