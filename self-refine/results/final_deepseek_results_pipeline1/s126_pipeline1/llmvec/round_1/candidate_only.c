#include <stdint.h>

typedef float float32x4_t __attribute__((__vector_size__(16)));

void vectorized_s126(int iterations, float bb[256][256], float cc[256][256], float * flat_2d_array)
{
    int total_iters = 10 * (iterations / 256);
    for (int nl = 0; nl < total_iters; nl++) {
        int k = 1;
        for (int i = 0; i < 256; i++) {
            int j = 1;
            // Vectorized main loop: process 4 columns at a time
            for (; j + 4 <= 256; j += 4) {
                float32x4_t bb_prev = { bb[j-1][i], bb[j][i], bb[j+1][i], bb[j+2][i] };
                float32x4_t cc_vals = { cc[j][i], cc[j+1][i], cc[j+2][i], cc[j+3][i] };
                float32x4_t flat_vals = { flat_2d_array[k-1], flat_2d_array[k], flat_2d_array[k+1], flat_2d_array[k+2] };
                float32x4_t result = bb_prev + flat_vals * cc_vals;
                bb[j][i] = result[0];
                bb[j+1][i] = result[1];
                bb[j+2][i] = result[2];
                bb[j+3][i] = result[3];
                k += 4;
            }
            // Scalar cleanup for remaining columns
            for (; j < 256; j++) {
                bb[j][i] = bb[j-1][i] + flat_2d_array[k-1] * cc[j][i];
                ++k;
            }
            ++k; // skip the last element of the row
        }
    }
}
