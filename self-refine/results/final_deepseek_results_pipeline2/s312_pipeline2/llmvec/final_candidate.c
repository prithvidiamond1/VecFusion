typedef float v8f32 __attribute__((__vector_size__(32), __aligned__(32)));

float vectorized_s312(int iterations, int LEN_1D, float* a) {
    float prod = (float)1.;
    int outer_loop_count = 10 * iterations;
    
    for (int nl = 0; nl < outer_loop_count; nl++) {
        float temp_prod = (float)1.;
        int i = 0;
        
        // Vectorized inner loop
        if (LEN_1D >= 8) {
            v8f32 vec_prod = (v8f32){1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
            int vector_limit = LEN_1D - (LEN_1D % 8);
            
            for (; i < vector_limit; i += 8) {
                v8f32 vec_a = *(v8f32*)&a[i];
                vec_prod *= vec_a;
            }
            
            // Horizontal reduction of vector product
            float* prod_elements = (float*)&vec_prod;
            for (int j = 0; j < 8; j++) {
                temp_prod *= prod_elements[j];
            }
        }
        
        // Scalar cleanup for remaining elements
        for (; i < LEN_1D; i++) {
            temp_prod *= a[i];
        }
        
        prod = temp_prod;
    }
    
    return prod;
}
