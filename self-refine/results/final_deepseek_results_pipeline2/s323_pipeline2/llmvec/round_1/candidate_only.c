typedef float float4 __attribute__((ext_vector_type(4)));

void vectorized_s323(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e) {
    int outer_iters = iterations / 2;
    
    for (int nl = 0; nl < outer_iters; nl++) {
        // Store original b[0] value for this iteration
        float b_prev = b[0];
        
        // First compute all c*d products (independent computations)
        int i = 1;
        int len_minus_1 = LEN_1D - 1;
        
        // Vectorized computation of c*d
        for (; i + 3 < LEN_1D; i += 4) {
            float4 c_vec = *(float4*)&c[i];
            float4 d_vec = *(float4*)&d[i];
            float4 cd_vec = c_vec * d_vec;
            *(float4*)&a[i] = cd_vec;
        }
        
        // Scalar tail for c*d computation
        for (; i < LEN_1D; i++) {
            a[i] = c[i] * d[i];
        }
        
        // Now compute the recurrence using stored products
        i = 1;
        
        // Main vectorized recurrence loop
        for (; i + 3 < LEN_1D; i += 4) {
            // Load c*d products (stored in a[i])
            float4 cd_vec = *(float4*)&a[i];
            
            // Load c and e values
            float4 c_vec = *(float4*)&c[i];
            float4 e_vec = *(float4*)&e[i];
            
            // Compute c*e products
            float4 ce_vec = c_vec * e_vec;
            
            // Process 4 elements sequentially to maintain recurrence
            float b0 = b_prev + cd_vec.x;
            float b1 = b0 + ce_vec.x;
            b[i] = b1;
            a[i] = b0;
            
            float b2 = b1 + cd_vec.y;
            float b3 = b2 + ce_vec.y;
            b[i + 1] = b3;
            a[i + 1] = b2;
            
            float b4 = b3 + cd_vec.z;
            float b5 = b4 + ce_vec.z;
            b[i + 2] = b5;
            a[i + 2] = b4;
            
            float b6 = b5 + cd_vec.w;
            float b7 = b6 + ce_vec.w;
            b[i + 3] = b7;
            a[i + 3] = b6;
            
            b_prev = b7;
        }
        
        // Scalar tail for recurrence computation
        for (; i < LEN_1D; i++) {
            float cd = a[i];
            float ce = c[i] * e[i];
            float a_new = b_prev + cd;
            b_prev = a_new + ce;
            b[i] = b_prev;
            a[i] = a_new;
        }
    }
}
