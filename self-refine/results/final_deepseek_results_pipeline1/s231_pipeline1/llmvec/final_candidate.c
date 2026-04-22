#include <stddef.h>

typedef float float_vec __attribute__((ext_vector_type(4)));

void vectorized_s231(int iterations, float aa[256][256], float bb[256][256]) {
    int total_iters = 100 * (iterations / 256);
    for (int nl = 0; nl < total_iters; nl++) {
        for (int i = 0; i < 256; ++i) {
            // Peel first iteration (j=1) for vectorization
            aa[1][i] = aa[0][i] + bb[1][i];
            
            // Vectorized loop: process 4 elements at a time
            for (int j = 2; j < 256; j += 4) {
                float_vec prev = *(float_vec*)&aa[j-1][i];
                float_vec b = *(float_vec*)&bb[j][i];
                float_vec result = prev + b;
                *(float_vec*)&aa[j][i] = result;
            }
            
            // Scalar cleanup for remaining elements
            for (int j = (256 - 2) / 4 * 4 + 2; j < 256; j++) {
                aa[j][i] = aa[j-1][i] + bb[j][i];
            }
        }
    }
}
