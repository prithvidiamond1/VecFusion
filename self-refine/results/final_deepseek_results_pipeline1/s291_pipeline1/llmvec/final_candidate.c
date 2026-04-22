#include <stdint.h>

void vectorized_s291(int iterations, int LEN_1D, float* a, float* b) {
    int total_outer = 2 * iterations;
    for (int nl = 0; nl < total_outer; nl++) {
        int im1 = LEN_1D - 1;
        int i = 0;
        // Vectorizable portion: process 4 elements at a time
        for (; i + 3 < LEN_1D; i += 4) {
            float b0 = b[i];
            float b1 = b[i+1];
            float b2 = b[i+2];
            float b3 = b[i+3];
            
            float b_im1_0 = b[im1];
            float b_im1_1 = b[i];
            float b_im1_2 = b[i+1];
            float b_im1_3 = b[i+2];
            
            a[i]   = (b0 + b_im1_0) * 0.5f;
            a[i+1] = (b1 + b_im1_1) * 0.5f;
            a[i+2] = (b2 + b_im1_2) * 0.5f;
            a[i+3] = (b3 + b_im1_3) * 0.5f;
            
            im1 = i + 3;
        }
        // Scalar cleanup for remaining elements
        for (; i < LEN_1D; i++) {
            a[i] = (b[i] + b[im1]) * 0.5f;
            im1 = i;
        }
    }
}
