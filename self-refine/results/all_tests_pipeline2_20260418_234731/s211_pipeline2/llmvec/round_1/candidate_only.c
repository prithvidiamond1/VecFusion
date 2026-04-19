#include <stdint.h>

void vectorized_s211(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e) {
    typedef float v4f __attribute__((vector_size(16)));
    const int VEC_SIZE = 4;
    
    for (int nl = 0; nl < iterations; nl++) {
        int i = 1;
        
        // Vectorized b[i] = b[i + 1] - e[i] * d[i];
        for (; i + VEC_SIZE <= LEN_1D - 1; i += VEC_SIZE) {
            v4f b_next = *(v4f*)&b[i + 1];
            v4f e_vec = *(v4f*)&e[i];
            v4f d_vec = *(v4f*)&d[i];
            v4f result = b_next - e_vec * d_vec;
            *(v4f*)&b[i] = result;
        }
        
        // Scalar tail for b updates
        for (; i < LEN_1D - 1; i++) {
            b[i] = b[i + 1] - e[i] * d[i];
        }
        
        i = 1;
        
        // Vectorized a[i] = b[i - 1] + c[i] * d[i];
        for (; i + VEC_SIZE <= LEN_1D - 1; i += VEC_SIZE) {
            v4f b_prev = *(v4f*)&b[i - 1];
            v4f c_vec = *(v4f*)&c[i];
            v4f d_vec = *(v4f*)&d[i];
            v4f result = b_prev + c_vec * d_vec;
            *(v4f*)&a[i] = result;
        }
        
        // Scalar tail for a updates
        for (; i < LEN_1D - 1; i++) {
            a[i] = b[i - 1] + c[i] * d[i];
        }
    }
}
