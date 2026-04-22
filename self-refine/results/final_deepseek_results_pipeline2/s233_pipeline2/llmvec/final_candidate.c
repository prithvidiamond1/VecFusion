#include <stdint.h>

void vectorized_s233(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    int nl_iterations = 100 * (iterations / 256);
    
    for (int nl = 0; nl < nl_iterations; nl++) {
        // Process aa with j outer, i inner - vectorizable along i dimension
        for (int j = 1; j < 256; j++) {
            int i = 1;
            // Vector loop
            for (; i <= 256 - 4; i += 4) {
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
        
        // Process bb with i outer, j inner - vectorizable along j dimension
        for (int i = 1; i < 256; i++) {
            int j = 1;
            // Vector loop
            for (; j <= 256 - 4; j += 4) {
                bb[j][i] = bb[j][i-1] + cc[j][i];
                bb[j+1][i] = bb[j+1][i-1] + cc[j+1][i];
                bb[j+2][i] = bb[j+2][i-1] + cc[j+2][i];
                bb[j+3][i] = bb[j+3][i-1] + cc[j+3][i];
            }
            // Scalar cleanup
            for (; j < 256; j++) {
                bb[j][i] = bb[j][i-1] + cc[j][i];
            }
        }
    }
}
