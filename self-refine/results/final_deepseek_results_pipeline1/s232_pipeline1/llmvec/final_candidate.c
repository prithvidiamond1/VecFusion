#include <stddef.h>

typedef float float_vec __attribute__((ext_vector_type(4)));

void vectorized_s232(int iterations, float aa[256][256], float bb[256][256])
{
    int outer_iters = 100 * (iterations / 256);
    for (int nl = 0; nl < outer_iters; nl++) {
        for (int j = 1; j < 256; j++) {
            int i = 1;
            // Vectorized main loop: process 4 elements at a time
            for (; i + 3 <= j; i += 4) {
                // Load previous values (aa[j][i-1] through aa[j][i+2])
                float_vec prev = *(float_vec *)&aa[j][i - 1];
                // Compute squares
                float_vec sq = prev * prev;
                // Load bb values
                float_vec b = *(float_vec *)&bb[j][i];
                // Compute new aa values
                float_vec new_vals = sq + b;
                // Store results
                *(float_vec *)&aa[j][i] = new_vals;
            }
            // Scalar cleanup for remaining elements
            for (; i <= j; i++) {
                aa[j][i] = aa[j][i-1] * aa[j][i-1] + bb[j][i];
            }
        }
    }
}
