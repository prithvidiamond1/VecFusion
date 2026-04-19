#include <stddef.h>

void vectorized_s141(int iterations, float * flat_2d_array, float bb[256][256]) {
    int nl_iter = 200 * (iterations / 256);
    
    for (int nl = 0; nl < nl_iter; nl++) {
        for (int i = 0; i < 256; i++) {
            float accum[256];
            
            // Vectorizable accumulation loop
            int j = i;
            for (; j <= 256 - 4; j += 4) {
                accum[j] = bb[j][i];
                accum[j + 1] = bb[j + 1][i];
                accum[j + 2] = bb[j + 2][i];
                accum[j + 3] = bb[j + 3][i];
            }
            // Scalar tail
            for (; j < 256; j++) {
                accum[j] = bb[j][i];
            }
            
            // Scatter loop remains scalar due to irregular access pattern
            int k = (i + 1) * i / 2 + i;
            for (int j = i; j < 256; j++) {
                flat_2d_array[k] += accum[j];
                k += j + 1;
            }
        }
    }
}
