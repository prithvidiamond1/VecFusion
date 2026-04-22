#include <stdlib.h>
#include <string.h>

typedef float v8f32 __attribute__((__vector_size__(32), __aligned__(32)));

void vectorized_s2251(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    const int VEC_SIZE = 8;
    
    for (int nl = 0; nl < iterations; nl++) {
        float s = (float)0.0;
        
        // Precompute tmp = b + c
        float* tmp = (float*)malloc(LEN_1D * sizeof(float));
        for (int i = 0; i < LEN_1D; i++) {
            tmp[i] = b[i] + c[i];
        }
        
        // First iteration (scalar)
        a[0] = s * e[0];
        b[0] = a[0] + d[0];
        s = tmp[0];
        
        // Main loop - process vector chunks with sequential recurrence
        int i = 1;
        for (; i + VEC_SIZE <= LEN_1D; i += VEC_SIZE) {
            // Process first element of chunk to get correct s for vector start
            a[i] = s * e[i];
            b[i] = a[i] + d[i];
            float s_start = tmp[i];
            
            // Load vectors for the remaining 7 elements
            v8f32 e_vec = {e[i+1], e[i+2], e[i+3], e[i+4], e[i+5], e[i+6], e[i+7], 0};
            v8f32 d_vec = {d[i+1], d[i+2], d[i+3], d[i+4], d[i+5], d[i+6], d[i+7], 0};
            v8f32 tmp_vec = {tmp[i+1], tmp[i+2], tmp[i+3], tmp[i+4], tmp[i+5], tmp[i+6], tmp[i+7], 0};
            
            // Create s vector: first element gets s_start, others get previous tmp
            v8f32 s_vec = {s_start, tmp[i+1], tmp[i+2], tmp[i+3], tmp[i+4], tmp[i+5], tmp[i+6], tmp[i+7]};
            
            // Compute a[i+1..i+7] = s_vec * e_vec
            v8f32 a_vec = s_vec * e_vec;
            
            // Store a vector results
            float a_store[8];
            __builtin_memcpy(a_store, &a_vec, sizeof(a_vec));
            for (int j = 1; j < VEC_SIZE; j++) {
                a[i + j] = a_store[j - 1];
            }
            
            // Compute b[i+1..i+7] = a_vec + d_vec
            v8f32 b_vec = a_vec + d_vec;
            
            // Store b vector results
            float b_store[8];
            __builtin_memcpy(b_store, &b_vec, sizeof(b_vec));
            for (int j = 1; j < VEC_SIZE; j++) {
                b[i + j] = b_store[j - 1];
            }
            
            // Update s for next chunk (last element's tmp value)
            s = tmp[i + VEC_SIZE - 1];
        }
        
        // Scalar tail for remaining elements
        for (; i < LEN_1D; i++) {
            a[i] = s * e[i];
            b[i] = a[i] + d[i];
            s = tmp[i];
        }
        
        free(tmp);
    }
}
