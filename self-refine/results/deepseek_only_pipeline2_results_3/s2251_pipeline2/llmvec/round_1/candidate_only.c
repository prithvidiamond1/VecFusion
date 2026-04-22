#include <stdlib.h>
#include <string.h>

typedef float v8f32 __attribute__((__vector_size__(32), __aligned__(32)));

void vectorized_s2251(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    const int VEC_SIZE = 8;
    
    for (int nl = 0; nl < iterations; nl++) {
        float s = (float)0.0;
        
        // Precompute b[i] + c[i] for all i
        float* tmp = (float*)malloc(LEN_1D * sizeof(float));
        
        // Vectorized precomputation
        int i = 0;
        for (; i + VEC_SIZE <= LEN_1D; i += VEC_SIZE) {
            v8f32 b_vec = *(v8f32*)&b[i];
            v8f32 c_vec = *(v8f32*)&c[i];
            v8f32 tmp_vec = b_vec + c_vec;
            *(v8f32*)&tmp[i] = tmp_vec;
        }
        // Scalar tail for precomputation
        for (; i < LEN_1D; i++) {
            tmp[i] = b[i] + c[i];
        }
        
        // First iteration (scalar)
        a[0] = s * e[0];
        b[0] = a[0] + d[0];
        s = tmp[0];
        
        // Main loop - vectorized with explicit dependency handling
        i = 1;
        for (; i + VEC_SIZE <= LEN_1D; i += VEC_SIZE) {
            // Broadcast s to vector
            v8f32 s_vec = (v8f32){s, s, s, s, s, s, s, s};
            
            // Load e[i..i+7]
            v8f32 e_vec = *(v8f32*)&e[i];
            
            // Compute a[i..i+7] = s * e[i..i+7]
            v8f32 a_vec = s_vec * e_vec;
            
            // Store a[i..i+7]
            *(v8f32*)&a[i] = a_vec;
            
            // Load d[i..i+7]
            v8f32 d_vec = *(v8f32*)&d[i];
            
            // Compute b[i..i+7] = a[i..i+7] + d[i..i+7]
            v8f32 b_vec = a_vec + d_vec;
            
            // Store b[i..i+7]
            *(v8f32*)&b[i] = b_vec;
            
            // Update s from tmp[i+7] (last element of current chunk)
            s = tmp[i + VEC_SIZE - 1];
        }
        
        // Scalar tail for main loop
        for (; i < LEN_1D; i++) {
            a[i] = s * e[i];
            b[i] = a[i] + d[i];
            s = tmp[i];
        }
        
        free(tmp);
    }
}
