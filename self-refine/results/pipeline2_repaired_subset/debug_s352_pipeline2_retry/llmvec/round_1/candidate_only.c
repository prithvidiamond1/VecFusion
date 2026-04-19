#include <stddef.h>

float vectorized_s352(int iterations, int LEN_1D, float* a, float *b) {
    float dot;
    for (int nl = 0; nl < 8*iterations; nl++) {
        dot = 0.f;
        
        // Vectorized main loop
        typedef float v4f __attribute__((vector_size(16)));
        int i;
        for (i = 0; i <= LEN_1D - 20; i += 20) {
            v4f dot_acc0 = {0.f, 0.f, 0.f, 0.f};
            v4f dot_acc1 = {0.f, 0.f, 0.f, 0.f};
            v4f dot_acc2 = {0.f, 0.f, 0.f, 0.f};
            v4f dot_acc3 = {0.f, 0.f, 0.f, 0.f};
            v4f dot_acc4 = {0.f, 0.f, 0.f, 0.f};
            
            // Process 5 groups of 4 elements each (total 20)
            v4f a_vec0 = *(v4f*)&a[i];
            v4f b_vec0 = *(v4f*)&b[i];
            dot_acc0 += a_vec0 * b_vec0;
            
            v4f a_vec1 = *(v4f*)&a[i+4];
            v4f b_vec1 = *(v4f*)&b[i+4];
            dot_acc1 += a_vec1 * b_vec1;
            
            v4f a_vec2 = *(v4f*)&a[i+8];
            v4f b_vec2 = *(v4f*)&b[i+8];
            dot_acc2 += a_vec2 * b_vec2;
            
            v4f a_vec3 = *(v4f*)&a[i+12];
            v4f b_vec3 = *(v4f*)&b[i+12];
            dot_acc3 += a_vec3 * b_vec3;
            
            v4f a_vec4 = *(v4f*)&a[i+16];
            v4f b_vec4 = *(v4f*)&b[i+16];
            dot_acc4 += a_vec4 * b_vec4;
            
            // Horizontal sum of all accumulators
            float* acc0 = (float*)&dot_acc0;
            float* acc1 = (float*)&dot_acc1;
            float* acc2 = (float*)&dot_acc2;
            float* acc3 = (float*)&dot_acc3;
            float* acc4 = (float*)&dot_acc4;
            
            dot += acc0[0] + acc0[1] + acc0[2] + acc0[3] +
                   acc1[0] + acc1[1] + acc1[2] + acc1[3] +
                   acc2[0] + acc2[1] + acc2[2] + acc2[3] +
                   acc3[0] + acc3[1] + acc3[2] + acc3[3] +
                   acc4[0] + acc4[1] + acc4[2] + acc4[3];
        }
        
        // Process remaining elements in groups of 5 (original pattern)
        for (; i <= LEN_1D - 5; i += 5) {
            float t0 = a[i] * b[i];
            float t1 = a[i+1] * b[i+1];
            float t2 = a[i+2] * b[i+2];
            float t3 = a[i+3] * b[i+3];
            float t4 = a[i+4] * b[i+4];
            dot += t0 + t1 + t2 + t3 + t4;
        }
        
        // Scalar tail for remaining elements
        for (; i < LEN_1D; i++) {
            dot += a[i] * b[i];
        }
    }
    return dot;
}
