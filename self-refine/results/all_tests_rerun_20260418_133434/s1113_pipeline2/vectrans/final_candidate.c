void s1113_opt(int iterations, int LEN_1D, float* a, float* b) {
    int mid = LEN_1D / 2;
    
    for (int nl = 0; nl < 2 * iterations; nl++) {
        // Pre-calculate the mid-point value for this iteration
        float a_mid_value = a[mid];
        
        // Process indices before mid using the original a[mid] value
        for (int i = 0; i < mid; i++) {
            a[i] = a_mid_value + b[i];
        }
        
        // Update the mid-point itself
        a[mid] = a_mid_value + b[mid];
        
        // Process indices after mid using the updated a[mid] value
        // Hoist the read of a[mid] outside the loop to enable vectorization
        float updated_mid_value = a[mid];
        for (int i = mid + 1; i < LEN_1D; i++) {
            a[i] = updated_mid_value + b[i];
        }
    }
}
