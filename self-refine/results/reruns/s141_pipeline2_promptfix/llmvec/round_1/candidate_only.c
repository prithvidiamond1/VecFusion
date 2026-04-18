#include <string.h>

void vectorized_s141(int iterations, float * flat_2d_array, float bb[256][256])
{
    int nl_iter = 200 * (iterations / 256);

    // Precompute all k indices for each (i,j) pair
    int k_indices[256][256];
    for (int i = 0; i < 256; i++) {
        int k = (i+1) * i / 2 + i;
        for (int j = i; j < 256; j++) {
            k_indices[i][j] = k;
            k += j + 1;
        }
    }

    for (int nl = 0; nl < nl_iter; nl++) {
        for (int i = 0; i < 256; i++) {
            float temp_accum[256];

            // Vectorizable: load bb[j][i] for j = i..255
            // This is a column access, stride = 256 floats
            // We unroll manually to help the compiler vectorize
            int j = i;
            int len = 256 - i;

            // Unrolled by 8 for vectorization hint
            int j8_end = i + (len & ~7);
            for (; j < j8_end; j += 8) {
                temp_accum[j+0] = bb[j+0][i];
                temp_accum[j+1] = bb[j+1][i];
                temp_accum[j+2] = bb[j+2][i];
                temp_accum[j+3] = bb[j+3][i];
                temp_accum[j+4] = bb[j+4][i];
                temp_accum[j+5] = bb[j+5][i];
                temp_accum[j+6] = bb[j+6][i];
                temp_accum[j+7] = bb[j+7][i];
            }
            for (; j < 256; j++) {
                temp_accum[j] = bb[j][i];
            }

            // Scatter: indirect addressing, hard to vectorize, keep scalar
            for (j = i; j < 256; j++) {
                int k = k_indices[i][j];
                flat_2d_array[k] += temp_accum[j];
            }
        }
    }
}
