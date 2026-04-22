typedef float float4 __attribute__((ext_vector_type(4)));

void vectorized_s453(int iterations, int LEN_1D, float* a, float *b) {
    for (int nl = 0; nl < iterations * 2; nl++) {
        int i = 0;
        const int vectorizable_elements = LEN_1D - (LEN_1D % 4);
        
        // Vectorized main loop
        for (; i < vectorizable_elements; i += 4) {
            float4 indices = {i + 1.0f, i + 2.0f, i + 3.0f, i + 4.0f};
            float4 scale = indices * 2.0f;
            
            float4 b_vec;
            b_vec[0] = b[i];
            b_vec[1] = b[i + 1];
            b_vec[2] = b[i + 2];
            b_vec[3] = b[i + 3];
            
            float4 result = scale * b_vec;
            
            a[i] = result[0];
            a[i + 1] = result[1];
            a[i + 2] = result[2];
            a[i + 3] = result[3];
        }
        
        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            a[i] = (2.0f * (i + 1)) * b[i];
        }
    }
}
