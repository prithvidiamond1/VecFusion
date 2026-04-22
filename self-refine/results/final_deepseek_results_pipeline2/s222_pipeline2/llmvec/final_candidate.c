typedef float float4 __attribute__((vector_size(16)));

void vectorized_s222(int iterations, int LEN_1D, float* a, float* b, float* c, float* e) {
    int outer_iters = iterations / 2;
    
    for (int nl = 0; nl < outer_iters; nl++) {
        // First loop: e[i] = e[i-1] * e[i-1] (sequential dependency)
        for (int i = 1; i < LEN_1D; i++) {
            e[i] = e[i - 1] * e[i - 1];
        }
        
        // Second loop: vectorizable operations
        int i = 1;
        int limit = LEN_1D;
        
        // Vector loop
        for (; i + 3 < limit; i += 4) {
            float4 b_vec = __builtin_convertvector(*(float4*)&b[i], float4);
            float4 c_vec = __builtin_convertvector(*(float4*)&c[i], float4);
            float4 a_vec = __builtin_convertvector(*(float4*)&a[i], float4);
            
            float4 temp_bc = b_vec * c_vec;
            a_vec += temp_bc;
            a_vec -= temp_bc;
            
            *(float4*)&a[i] = __builtin_convertvector(a_vec, float4);
        }
        
        // Scalar tail
        for (; i < limit; i++) {
            float temp_bc = b[i] * c[i];
            a[i] += temp_bc;
            a[i] -= temp_bc;
        }
    }
}
