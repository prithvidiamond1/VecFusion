#include <stdint.h>

void vectorized_s2233(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    int loop_count = 100 * (iterations / 256);
    
    for (int nl = 0; nl < loop_count; nl++) {
        // Process aa updates column by column
        for (int i = 1; i < 256; i++) {
            // Vectorize the inner j loop
            int j = 1;
            // Process in chunks of 4
            for (; j + 3 < 256; j += 4) {
                // Load previous row values
                float prev0 = aa[j-1][i];
                float prev1 = aa[j][i];
                float prev2 = aa[j+1][i];
                float prev3 = aa[j+2][i];
                
                // Load cc values
                float cc0 = cc[j][i];
                float cc1 = cc[j+1][i];
                float cc2 = cc[j+2][i];
                float cc3 = cc[j+3][i];
                
                // Compute new values
                aa[j][i] = prev0 + cc0;
                aa[j+1][i] = prev1 + cc1;
                aa[j+2][i] = prev2 + cc2;
                aa[j+3][i] = prev3 + cc3;
            }
            // Scalar tail
            for (; j < 256; j++) {
                aa[j][i] = aa[j-1][i] + cc[j][i];
            }
        }
        
        // Process bb updates row by row
        for (int i = 1; i < 256; i++) {
            // Vectorize the inner j loop
            int j = 1;
            // Process in chunks of 4
            for (; j + 3 < 256; j += 4) {
                // Load previous column values
                float prev0 = bb[i-1][j];
                float prev1 = bb[i-1][j+1];
                float prev2 = bb[i-1][j+2];
                float prev3 = bb[i-1][j+3];
                
                // Load cc values
                float cc0 = cc[i][j];
                float cc1 = cc[i][j+1];
                float cc2 = cc[i][j+2];
                float cc3 = cc[i][j+3];
                
                // Compute new values
                bb[i][j] = prev0 + cc0;
                bb[i][j+1] = prev1 + cc1;
                bb[i][j+2] = prev2 + cc2;
                bb[i][j+3] = prev3 + cc3;
            }
            // Scalar tail
            for (; j < 256; j++) {
                bb[i][j] = bb[i-1][j] + cc[i][j];
            }
        }
    }
}
