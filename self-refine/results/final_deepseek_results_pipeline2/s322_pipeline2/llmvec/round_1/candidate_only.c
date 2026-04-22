typedef float float4 __attribute__((ext_vector_type(4)));

void vectorized_s322(int iterations, int LEN_1D, float* a, float* b, float* c) {
    int loop_count = iterations / 2;
    
    for (int nl = 0; nl < loop_count; nl++) {
        // Store original values for scalar replacement
        float prev2 = a[0];
        float prev1 = a[1];
        
        // Process elements 2..LEN_1D-1
        int i = 2;
        
        // Vectorized main loop (process 4 elements at a time)
        for (; i + 3 < LEN_1D; i += 4) {
            // Load current a values
            float4 current_vec = {a[i], a[i+1], a[i+2], a[i+3]};
            
            // Load b and c values
            float4 b_vec = {b[i], b[i+1], b[i+2], b[i+3]};
            float4 c_vec = {c[i], c[i+1], c[i+2], c[i+3]};
            
            // Compute results sequentially within the vector
            float results[4];
            
            // First element in vector
            results[0] = current_vec.x + prev1 * b_vec.x + prev2 * c_vec.x;
            prev2 = prev1;
            prev1 = results[0];
            
            // Second element
            results[1] = current_vec.y + prev1 * b_vec.y + prev2 * c_vec.y;
            prev2 = prev1;
            prev1 = results[1];
            
            // Third element
            results[2] = current_vec.z + prev1 * b_vec.z + prev2 * c_vec.z;
            prev2 = prev1;
            prev1 = results[2];
            
            // Fourth element
            results[3] = current_vec.w + prev1 * b_vec.w + prev2 * c_vec.w;
            prev2 = prev1;
            prev1 = results[3];
            
            // Store results back to a
            a[i] = results[0];
            a[i+1] = results[1];
            a[i+2] = results[2];
            a[i+3] = results[3];
        }
        
        // Scalar cleanup for remaining elements
        for (; i < LEN_1D; i++) {
            float current = a[i];
            float temp = current + prev1 * b[i] + prev2 * c[i];
            prev2 = prev1;
            prev1 = temp;
            a[i] = temp;
        }
    }
}
