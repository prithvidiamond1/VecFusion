typedef float float4 __attribute__((ext_vector_type(4)));

void vectorized_s453(int iterations, int LEN_1D, float* a, float *b)
{
    int total_iterations = iterations * 2;
    
    for (int nl = 0; nl < total_iterations; nl++) {
        int i = 0;
        
        // Vectorized main loop
        for (; i + 3 < LEN_1D; i += 4) {
            float4 indices = {i + 1.0f, i + 2.0f, i + 3.0f, i + 4.0f};
            float4 scale = indices * 2.0f;
            
            float4 b_vec = {b[i], b[i+1], b[i+2], b[i+3]};
            float4 result = scale * b_vec;
            
            a[i] = result.x;
            a[i+1] = result.y;
            a[i+2] = result.z;
            a[i+3] = result.w;
        }
        
        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            float s = 2.0f * (i + 1);
            a[i] = s * b[i];
        }
    }
}
