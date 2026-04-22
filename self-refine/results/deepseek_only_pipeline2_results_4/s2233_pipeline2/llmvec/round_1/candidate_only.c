#include <stddef.h>

void vectorized_s2233(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    int loop_count = 100 * (iterations / 256);
    
    for (int nl = 0; nl < loop_count; nl++) {
        // Process aa updates - vectorize inner i loop
        for (int j = 1; j < 256; j++) {
            int i;
            // Vectorized main loop
            for (i = 1; i <= 256 - 4; i += 4) {
                aa[j][i] = aa[j-1][i] + cc[j][i];
                aa[j][i+1] = aa[j-1][i+1] + cc[j][i+1];
                aa[j][i+2] = aa[j-1][i+2] + cc[j][i+2];
                aa[j][i+3] = aa[j-1][i+3] + cc[j][i+3];
            }
            // Scalar cleanup
            for (; i < 256; i++) {
                aa[j][i] = aa[j-1][i] + cc[j][i];
            }
        }

        // Process bb updates - vectorize inner j loop
        for (int i = 1; i < 256; i++) {
            int j;
            // Vectorized main loop
            for (j = 1; j <= 256 - 4; j += 4) {
                bb[i][j] = bb[i-1][j] + cc[i][j];
                bb[i][j+1] = bb[i-1][j+1] + cc[i][j+1];
                bb[i][j+2] = bb[i-1][j+2] + cc[i][j+2];
                bb[i][j+3] = bb[i-1][j+3] + cc[i][j+3];
            }
            // Scalar cleanup
            for (; j < 256; j++) {
                bb[i][j] = bb[i-1][j] + cc[i][j];
            }
        }
    }
}
