#include <stddef.h>

typedef float float_vec __attribute__((__vector_size__(16)));

void vectorized_s222(int iterations, int LEN_1D, float *a, float *b, float *c, float *e) {
    int total_iters = iterations / 2;
    if (total_iters <= 0 || LEN_1D <= 1) return;
    
    for (int nl = 0; nl < total_iters; nl++) {
        int i = 1;
        // Handle first element separately due to e[i-1] dependency
        float e_prev = e[0];
        for (; i < LEN_1D; i++) {
            float bc = b[i] * c[i];
            a[i] += bc;
            float e_sq = e_prev * e_prev;
            e[i] = e_sq;
            a[i] -= bc;
            e_prev = e_sq;
        }
    }
}
