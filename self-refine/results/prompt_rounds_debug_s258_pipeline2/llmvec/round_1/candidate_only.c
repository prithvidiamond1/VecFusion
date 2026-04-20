#include <stdint.h>

void vectorized_s258(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256]) {
    typedef float v8f __attribute__((ext_vector_type(8)));
    typedef int v8i __attribute__((ext_vector_type(8)));
    
    const v8f vzero = (v8f){0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    const v8f vone = (v8f){1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    
    for (int nl = 0; nl < iterations; nl++) {
        float s = 0.0f;
        float s_vals[256];
        
        // First loop: compute s values with dependency - vectorized with horizontal dependency
        int i = 0;
        for (; i <= 248; i += 8) {
            v8f a_vec = *((v8f*)&a[i]);
            v8f d_vec = *((v8f*)&d[i]);
            
            // Compute condition mask
            v8f condition = __builtin_convertvector(a_vec > vzero, v8f);
            v8f one_minus_condition = vone - condition;
            
            // Compute d[i] * d[i]
            v8f d_sq = d_vec * d_vec;
            
            // Compute condition * (d[i] * d[i]) + (1 - condition) * s
            // s is scalar, need to broadcast it
            v8f s_broadcast = (v8f){s, s, s, s, s, s, s, s};
            v8f new_s_vec = condition * d_sq + one_minus_condition * s_broadcast;
            
            // Store results sequentially with dependency
            float* new_s_ptr = (float*)&new_s_vec;
            for (int j = 0; j < 8; j++) {
                s = new_s_ptr[j];
                s_vals[i + j] = s;
            }
        }
        
        // Scalar tail for first loop
        for (; i < 256; i++) {
            float condition = a[i] > 0.0f ? 1.0f : 0.0f;
            s = condition * (d[i] * d[i]) + (1.0f - condition) * s;
            s_vals[i] = s;
        }
        
        // Second loop: compute b[i] using stored s values - fully vectorizable
        i = 0;
        for (; i <= 248; i += 8) {
            v8f s_vec = *((v8f*)&s_vals[i]);
            v8f c_vec = *((v8f*)&c[i]);
            v8f d_vec = *((v8f*)&d[i]);
            
            v8f b_vec = s_vec * c_vec + d_vec;
            *((v8f*)&b[i]) = b_vec;
        }
        
        // Scalar tail for second loop
        for (; i < 256; i++) {
            b[i] = s_vals[i] * c[i] + d[i];
        }
        
        // Third loop: compute e[i] using stored s values - fully vectorizable
        i = 0;
        for (; i <= 248; i += 8) {
            v8f s_vec = *((v8f*)&s_vals[i]);
            v8f aa_vec = *((v8f*)&aa[0][i]);
            
            v8f e_vec = (s_vec + vone) * aa_vec;
            *((v8f*)&e[i]) = e_vec;
        }
        
        // Scalar tail for third loop
        for (; i < 256; i++) {
            e[i] = (s_vals[i] + 1.0f) * aa[0][i];
        }
    }
}
