typedef float v8f32 __attribute__((__vector_size__(32), __aligned__(4)));

void vectorized_s341(int iterations, int LEN_1D, float* a, float* b) {
    const int VEC_SIZE = 8;
    
    for (int nl = 0; nl < iterations; nl++) {
        int write_idx = 0;
        int i = 0;
        
        // Vectorized main loop
        for (; i + VEC_SIZE <= LEN_1D; i += VEC_SIZE) {
            v8f32 b_vec = *(v8f32*)&b[i];
            v8f32 mask_vec = b_vec > (v8f32){0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
            
            // Extract mask bits
            int mask = __builtin_convertvector(mask_vec, v8f32) != (v8f32){0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
            
            // Process each element based on mask
            for (int j = 0; j < VEC_SIZE; j++) {
                if (mask & (1 << j)) {
                    a[write_idx++] = b[i + j];
                }
            }
        }
        
        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            float temp_b = b[i];
            if (temp_b > 0.0f) {
                a[write_idx] = temp_b;
                write_idx += 1;
            }
        }
    }
}
