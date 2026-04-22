#include <stddef.h>

typedef float v4sf __attribute__((__vector_size__(16)));

void vectorized_s275(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    int total_iterations = 10 * (iterations / 256);
    for (int nl = 0; nl < total_iterations; nl++) {
        for (int i = 0; i < 256; i++) {
            if (aa[0][i] > 0.0f) {
                int j = 1;
                // Vectorized loop: process 4 elements at a time
                for (; j + 4 <= 256; j += 4) {
                    v4sf aa_prev = { aa[j-1][i], aa[j][i], aa[j+1][i], aa[j+2][i] };
                    v4sf bb_vec = { bb[j][i], bb[j+1][i], bb[j+2][i], bb[j+3][i] };
                    v4sf cc_vec = { cc[j][i], cc[j+1][i], cc[j+2][i], cc[j+3][i] };
                    v4sf prod = bb_vec * cc_vec;
                    v4sf aa_new = aa_prev + prod;
                    aa[j][i] = aa_new[0];
                    aa[j+1][i] = aa_new[1];
                    aa[j+2][i] = aa_new[2];
                    aa[j+3][i] = aa_new[3];
                }
                // Scalar cleanup for remaining elements
                for (; j < 256; j++) {
                    aa[j][i] = aa[j-1][i] + bb[j][i] * cc[j][i];
                }
            }
        }
    }
}
