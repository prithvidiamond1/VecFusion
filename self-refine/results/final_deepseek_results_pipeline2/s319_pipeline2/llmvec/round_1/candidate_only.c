typedef float float4 __attribute__((ext_vector_type(4)));

float vectorized_s319(int iterations, int LEN_1D, float *a, float *b, float *c, float *d, float *e) {
    float sum = 0.0f;
    
    for (int nl = 0; nl < 2 * iterations; nl++) {
        float sum_a = 0.0f;
        float sum_b = 0.0f;
        
        // Vectorized first loop
        int i = 0;
        for (; i + 3 < LEN_1D; i += 4) {
            float4 c_vec = *(float4 *)&c[i];
            float4 d_vec = *(float4 *)&d[i];
            float4 a_vec = c_vec + d_vec;
            *(float4 *)&a[i] = a_vec;
            
            // Horizontal sum of a_vec
            sum_a += a_vec.x + a_vec.y + a_vec.z + a_vec.w;
        }
        
        // Scalar tail for first loop
        for (; i < LEN_1D; i++) {
            a[i] = c[i] + d[i];
            sum_a += a[i];
        }
        
        // Vectorized second loop
        i = 0;
        for (; i + 3 < LEN_1D; i += 4) {
            float4 c_vec = *(float4 *)&c[i];
            float4 e_vec = *(float4 *)&e[i];
            float4 b_vec = c_vec + e_vec;
            *(float4 *)&b[i] = b_vec;
            
            // Horizontal sum of b_vec
            sum_b += b_vec.x + b_vec.y + b_vec.z + b_vec.w;
        }
        
        // Scalar tail for second loop
        for (; i < LEN_1D; i++) {
            b[i] = c[i] + e[i];
            sum_b += b[i];
        }
        
        sum = sum_a + sum_b;
    }
    
    return sum;
}
