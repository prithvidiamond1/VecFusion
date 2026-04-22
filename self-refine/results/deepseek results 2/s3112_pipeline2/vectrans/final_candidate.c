float s3112_opt(int iterations, int LEN_1D, float* a, float* b)
{
    float sum;
    for (int nl = 0; nl < iterations; nl++) {
        sum = (float)0.0;
        
        // Loop distribution: separate the reduction from the store
        // First compute all prefix sums into a temporary array
        float temp_sum = sum;
        
        // Use manual unrolling to expose independent operations
        int i = 0;
        for (; i + 3 < LEN_1D; i += 4) {
            // Load all inputs first (no dependencies)
            float t0 = a[i];
            float t1 = a[i+1];
            float t2 = a[i+2];
            float t3 = a[i+3];
            
            // Compute prefix sums sequentially but with explicit dependency chain
            float s0 = temp_sum + t0;
            float s1 = s0 + t1;
            float s2 = s1 + t2;
            float s3 = s2 + t3;
            
            // Store all results
            b[i] = s0;
            b[i+1] = s1;
            b[i+2] = s2;
            b[i+3] = s3;
            
            // Update accumulator for next chunk
            temp_sum = s3;
        }
        
        // Handle remaining elements
        for (; i < LEN_1D; i++) {
            temp_sum += a[i];
            b[i] = temp_sum;
        }
        
        sum = temp_sum;
    }
    return sum;
}
