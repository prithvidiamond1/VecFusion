#include <stdint.h>

float vectorized_s352(int iterations, int LEN_1D, float* a, float *b) {
    float dot;
    
    for (int nl = 0; nl < 8 * iterations; nl++) {
        dot = 0.0f;
        
        // Vectorized accumulation
        typedef float v8f __attribute__((vector_size(32), aligned(1)));
        v8f vdot = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
        
        // Process 8 elements at a time
        int i = 0;
        int main_len = LEN_1D - (LEN_1D % 8);
        
        for (; i < main_len; i += 8) {
            v8f va = *(v8f*)&a[i];
            v8f vb = *(v8f*)&b[i];
            vdot += va * vb;
        }
        
        // Horizontal sum of vector accumulator
        float* vdot_ptr = (float*)&vdot;
        dot = vdot_ptr[0] + vdot_ptr[1] + vdot_ptr[2] + vdot_ptr[3] +
              vdot_ptr[4] + vdot_ptr[5] + vdot_ptr[6] + vdot_ptr[7];
        
        // Scalar tail for remaining elements
        for (; i < LEN_1D; i++) {
            dot += a[i] * b[i];
        }
    }
    
    return dot;
}
