#include <stdint.h>

void vectorized_s232(int iterations, float aa[256][256], float bb[256][256])
{
    int loop_count = 100 * (iterations / 256);
    
    for (int nl = 0; nl < loop_count; nl++) {
        // Handle i=1 separately for all j
        for (int j = 1; j < 256; j++) {
            if (j >= 1) {
                aa[j][1] = aa[j][0] * aa[j][0] + bb[j][1];
            }
        }

        // Process remaining i values
        for (int i = 2; i < 256; i++) {
            // Vectorize the inner j loop
            int j = i;
            // Process in chunks of 4
            for (; j + 3 < 256; j += 4) {
                float a0 = aa[j][i-1];
                float a1 = aa[j+1][i-1];
                float a2 = aa[j+2][i-1];
                float a3 = aa[j+3][i-1];
                
                aa[j][i] = a0 * a0 + bb[j][i];
                aa[j+1][i] = a1 * a1 + bb[j+1][i];
                aa[j+2][i] = a2 * a2 + bb[j+2][i];
                aa[j+3][i] = a3 * a3 + bb[j+3][i];
            }
            // Scalar cleanup
            for (; j < 256; j++) {
                aa[j][i] = aa[j][i-1] * aa[j][i-1] + bb[j][i];
            }
        }
    }
}
