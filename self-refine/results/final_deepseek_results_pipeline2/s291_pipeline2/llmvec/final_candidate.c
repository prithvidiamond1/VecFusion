typedef float float4 __attribute__((ext_vector_type(4)));

void vectorized_s291(int iterations, int LEN_1D, float* a, float* b) {
    int total_outer_iterations = 2 * iterations;
    
    for (int nl = 0; nl < total_outer_iterations; nl++) {
        int im1 = LEN_1D - 1;
        float prev_b = b[im1];
        
        // Vectorized main loop
        int i = 0;
        int vec_limit = LEN_1D - (LEN_1D % 4);
        
        if (vec_limit > 0) {
            // Load first vector with proper alignment for the recurrence
            float4 current_vec = *(float4*)&b[0];
            float prev_scalar = prev_b;
            
            for (; i < vec_limit; i += 4) {
                // Create vector of previous values: [prev_scalar, current_vec[0], current_vec[1], current_vec[2]]
                float4 prev_vec = {prev_scalar, current_vec[0], current_vec[1], current_vec[2]};
                
                // Compute average
                float4 avg_vec = (current_vec + prev_vec) * 0.5f;
                
                // Store result
                *(float4*)&a[i] = avg_vec;
                
                // Update prev_scalar for next iteration
                prev_scalar = current_vec[3];
                
                // Load next vector if not at the end
                if (i + 4 < vec_limit) {
                    current_vec = *(float4*)&b[i + 4];
                }
            }
            
            // Update prev_b for scalar tail
            prev_b = prev_scalar;
        }
        
        // Scalar cleanup for remaining elements
        for (; i < LEN_1D; i++) {
            float current_b = b[i];
            a[i] = (current_b + prev_b) * 0.5f;
            prev_b = current_b;
        }
    }
}
