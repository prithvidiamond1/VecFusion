#include <stdint.h>

void vectorized_s244(int iterations, int LEN_1D, float* a, float* b, float* c, float* d) {
    typedef float v4f __attribute__((vector_size(16)));
    const int VF = 4;

    for (int nl = 0; nl < iterations; nl++) {
        // Vectorize b[i] = c[i] + b[i]
        int i = 0;
        for (; i <= LEN_1D - 1 - VF; i += VF) {
            v4f b_vec = *(v4f*)&b[i];
            v4f c_vec = *(v4f*)&c[i];
            v4f result = b_vec + c_vec;
            *(v4f*)&b[i] = result;
        }
        // Scalar tail for b updates
        for (; i < LEN_1D - 1; ++i) {
            b[i] = c[i] + b[i];
        }

        // Handle first iteration separately
        i = 0;
        float a_next_temp = a[i+1];
        a[i] = b[i] + c[i] * d[i];
        a[i+1] = b[i] + a_next_temp * d[i];

        // Vectorize main loop
        for (i = 1; i <= LEN_1D - 2 - VF; i += VF) {
            // Load b[i] for current vector
            v4f b_vec = *(v4f*)&b[i];
            
            // Load c[i] and d[i]
            v4f c_vec = *(v4f*)&c[i];
            v4f d_vec = *(v4f*)&d[i];
            
            // Compute and store a[i] = b[i] + c[i] * d[i]
            v4f a_current = b_vec + c_vec * d_vec;
            *(v4f*)&a[i] = a_current;
            
            // Load original a[i+1] values (from memory, not overwritten yet)
            v4f a_next_orig = *(v4f*)&a[i+1];
            
            // Compute and store a[i+1] = b[i] + a_next_orig * d[i]
            v4f a_next_result = b_vec + a_next_orig * d_vec;
            *(v4f*)&a[i+1] = a_next_result;
        }

        // Scalar tail for main loop
        for (; i < LEN_1D - 1; ++i) {
            float a_next_temp = a[i+1];
            a[i] = b[i] + c[i] * d[i];
            a[i+1] = b[i] + a_next_temp * d[i];
        }
    }
}
