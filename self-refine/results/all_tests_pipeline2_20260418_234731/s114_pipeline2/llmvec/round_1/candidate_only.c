#include <stddef.h>

void vectorized_s114(int iterations, float aa[256][256], float bb[256][256]) {
    int loop_count = 200 * (iterations / 256);
    
    for (int nl = 0; nl < loop_count; nl++) {
        float temp_lower[256][256];
        
        // Compute lower triangle into temp_lower
        for (int i = 0; i < 256; i++) {
            int j = 0;
            // Vectorized chunk processing
            for (; j + 3 < i; j += 4) {
                // Load aa[j:j+3][i] - these are contiguous in memory
                float aa0 = aa[j][i];
                float aa1 = aa[j+1][i];
                float aa2 = aa[j+2][i];
                float aa3 = aa[j+3][i];
                
                // Load bb[i][j:j+3] - these are contiguous in memory
                float bb0 = bb[i][j];
                float bb1 = bb[i][j+1];
                float bb2 = bb[i][j+2];
                float bb3 = bb[i][j+3];
                
                // Compute and store
                temp_lower[i][j] = aa0 + bb0;
                temp_lower[i][j+1] = aa1 + bb1;
                temp_lower[i][j+2] = aa2 + bb2;
                temp_lower[i][j+3] = aa3 + bb3;
            }
            
            // Scalar cleanup for remaining elements
            for (; j < i; j++) {
                temp_lower[i][j] = aa[j][i] + bb[i][j];
            }
        }
        
        // Copy temp_lower back to aa
        for (int i = 0; i < 256; i++) {
            int j = 0;
            // Vectorized chunk processing
            for (; j + 3 < i; j += 4) {
                // Load temp_lower[i][j:j+3] - contiguous
                float t0 = temp_lower[i][j];
                float t1 = temp_lower[i][j+1];
                float t2 = temp_lower[i][j+2];
                float t3 = temp_lower[i][j+3];
                
                // Store to aa[i][j:j+3] - contiguous
                aa[i][j] = t0;
                aa[i][j+1] = t1;
                aa[i][j+2] = t2;
                aa[i][j+3] = t3;
            }
            
            // Scalar cleanup for remaining elements
            for (; j < i; j++) {
                aa[i][j] = temp_lower[i][j];
            }
        }
    }
}
