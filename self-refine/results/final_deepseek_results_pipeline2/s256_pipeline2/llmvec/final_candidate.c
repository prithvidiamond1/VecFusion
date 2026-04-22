#include <stdint.h>

void vectorized_s256(int iterations, float* a, float* d, float aa[256][256], float bb[256][256]) {
    int outer_iters = 10 * (iterations / 256);
    float a_prev[256];
    
    for (int nl = 0; nl < outer_iters; nl++) {
        // Store initial a values
        for (int k = 0; k < 256; k++) {
            a_prev[k] = a[k];
        }
        
        // Process all i for each j
        for (int j = 1; j < 256; j++) {
            // Compute a[j] once per j
            a[j] = 1.0f - a[j - 1];
            
            // Vectorizable loop over i with explicit bounds
            float aj = a[j];
            float dj = d[j];
            float* aa_row = aa[j];
            float* bb_row = bb[j];
            
            // Process 8 elements at a time (256-bit vectors)
            int i = 0;
            for (; i + 7 < 256; i += 8) {
                // Load 8 bb values
                float bb0 = bb_row[i];
                float bb1 = bb_row[i + 1];
                float bb2 = bb_row[i + 2];
                float bb3 = bb_row[i + 3];
                float bb4 = bb_row[i + 4];
                float bb5 = bb_row[i + 5];
                float bb6 = bb_row[i + 6];
                float bb7 = bb_row[i + 7];
                
                // Compute bb[j][i] * d[j]
                float prod0 = bb0 * dj;
                float prod1 = bb1 * dj;
                float prod2 = bb2 * dj;
                float prod3 = bb3 * dj;
                float prod4 = bb4 * dj;
                float prod5 = bb5 * dj;
                float prod6 = bb6 * dj;
                float prod7 = bb7 * dj;
                
                // Compute a[j] + bb[j][i] * d[j]
                aa_row[i] = aj + prod0;
                aa_row[i + 1] = aj + prod1;
                aa_row[i + 2] = aj + prod2;
                aa_row[i + 3] = aj + prod3;
                aa_row[i + 4] = aj + prod4;
                aa_row[i + 5] = aj + prod5;
                aa_row[i + 6] = aj + prod6;
                aa_row[i + 7] = aj + prod7;
            }
            
            // Scalar cleanup for remaining elements
            for (; i < 256; i++) {
                aa_row[i] = aj + bb_row[i] * dj;
            }
        }
        
        // Restore a[0] for next iteration
        a[0] = a_prev[0];
    }
}
