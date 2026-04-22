#include <stdint.h>

void vectorized_s232(int iterations, float aa[256][256], float bb[256][256])
{
    int loop_count = 100 * (iterations / 256);
    
    for (int nl = 0; nl < loop_count; nl++) {
        for (int j = 1; j < 256; j++) {
            // Process vectorizable portion with explicit bounds
            int i = 1;
            // Vectorize inner loop where possible
            for (; i + 3 <= j; i += 4) {
                // Load previous values (scalar expansion for vectorization)
                float prev0 = aa[j][i-1];
                float prev1 = prev0 * prev0 + bb[j][i];
                float prev2 = prev1 * prev1 + bb[j][i+1];
                float prev3 = prev2 * prev2 + bb[j][i+2];
                float prev4 = prev3 * prev3 + bb[j][i+3];
                
                // Store results
                aa[j][i] = prev1;
                aa[j][i+1] = prev2;
                aa[j][i+2] = prev3;
                aa[j][i+3] = prev4;
                
                // Update prev for next iteration
                aa[j][i-1] = prev0; // Already stored from previous iteration
            }
            
            // Scalar cleanup for remaining iterations
            float prev = aa[j][i-1];
            for (; i <= j; i++) {
                float current = prev * prev + bb[j][i];
                aa[j][i] = current;
                prev = current;
            }
        }
    }
}
