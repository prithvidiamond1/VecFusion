#include <stddef.h>

typedef float float_array_256[256] __attribute__((aligned(16)));

void vectorized_s2233(int iterations, float aa[256][256], float bb[256][256], float cc[256][256]) {
    int total_iterations = 100 * (iterations / 256);
    for (int nl = 0; nl < total_iterations; nl++) {
        for (int i = 1; i < 256; i++) {
            // Vectorize the first inner loop (j dimension)
            for (int j = 1; j < 256; j += 4) {
                // Process 4 elements at a time
                for (int k = 0; k < 4 && j + k < 256; k++) {
                    aa[j + k][i] = aa[j + k - 1][i] + cc[j + k][i];
                }
            }
            
            // Vectorize the second inner loop (j dimension)
            for (int j = 1; j < 256; j += 4) {
                // Process 4 elements at a time
                for (int k = 0; k < 4 && j + k < 256; k++) {
                    bb[i][j + k] = bb[i - 1][j + k] + cc[i][j + k];
                }
            }
        }
    }
}
