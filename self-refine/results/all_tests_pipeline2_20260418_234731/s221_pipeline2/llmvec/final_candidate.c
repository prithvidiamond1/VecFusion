#include <stdint.h>

void vectorized_s221(int iterations, int LEN_1D, float* a, float* b, float* c, float* d) {
    typedef float v4f __attribute__((vector_size(16)));
    typedef int v4i __attribute__((vector_size(16)));
    
    int outer_iters = iterations / 2;
    
    for (int nl = 0; nl < outer_iters; nl++) {
        // First loop: vectorizable a[i] += c[i] * d[i]
        int i = 1;
        for (; i + 3 < LEN_1D; i += 4) {
            v4f a_vec = *(v4f*)&a[i];
            v4f c_vec = *(v4f*)&c[i];
            v4f d_vec = *(v4f*)&d[i];
            a_vec += c_vec * d_vec;
            *(v4f*)&a[i] = a_vec;
        }
        // Scalar tail for first loop
        for (; i < LEN_1D; i++) {
            a[i] += c[i] * d[i];
        }
        
        // Second loop: loop-carried dependency on b[i-1]
        // Process sequentially as vectorization would break dependency
        for (i = 1; i < LEN_1D; i++) {
            b[i] = b[i - 1] + a[i] + d[i];
        }
    }
}
