typedef float float4 __attribute__((ext_vector_type(4)));

void vectorized_s321(int iterations, int LEN_1D, float* a, float* b) {
    for (int nl = 0; nl < iterations; nl++) {
        float prev = a[0];
        
        // Process first element separately to establish prev
        int i = 1;
        
        // Main vectorized loop
        for (; i + 3 < LEN_1D; i += 4) {
            // Load current block
            float4 a_vec = {a[i], a[i+1], a[i+2], a[i+3]};
            float4 b_vec = {b[i], b[i+1], b[i+2], b[i+3]};
            
            // Compute: a[i] = a[i] + prev * b[i]
            // Then prev becomes the new a[i] for next iteration
            float4 prev_vec = {prev, a[i], a[i+1], a[i+2]};
            float4 result = a_vec + prev_vec * b_vec;
            
            // Store results
            a[i] = result[0];
            a[i+1] = result[1];
            a[i+2] = result[2];
            a[i+3] = result[3];
            
            // Update prev for next iteration
            prev = result[3];
        }
        
        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            float temp = a[i];
            a[i] = temp + prev * b[i];
            prev = a[i];
        }
    }
}
