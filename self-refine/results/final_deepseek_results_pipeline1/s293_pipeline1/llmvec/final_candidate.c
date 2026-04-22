#include <stddef.h>

typedef float float_vec __attribute__((__vector_size__(16)));

void vectorized_s293(int iterations, int LEN_1D, float* a) {
    int total_iters = 4 * iterations;
    for (int nl = 0; nl < total_iters; nl++) {
        float val = a[0];
        int i = 0;
        // Vectorized loop: process 4 floats at a time
        float_vec vec_val = (float_vec){val, val, val, val};
        for (; i + 4 <= LEN_1D; i += 4) {
            *(float_vec*)(a + i) = vec_val;
        }
        // Scalar cleanup for remaining elements
        for (; i < LEN_1D; i++) {
            a[i] = val;
        }
    }
}
