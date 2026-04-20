float s3112_opt(int iterations, int LEN_1D, float* a, float* b)
{
    float sum;
    for (int nl = 0; nl < iterations; nl++) {
        sum = (float)0.0;
        
        // First loop: compute reduction (vectorizable)
        float temp_sum = (float)0.0;
        for (int i = 0; i < LEN_1D; i++) {
            temp_sum += a[i];
        }
        sum = temp_sum;
        
        // Second loop: compute prefix sum with reordered memory access
        // Store original a[i] before updating prefix_sum to break dependency
        float prefix_sum = (float)0.0;
        for (int i = 0; i < LEN_1D; i++) {
            float current_a = a[i];  // Load a[i] first
            prefix_sum += current_a; // Then update prefix_sum
            b[i] = prefix_sum;       // Finally store to b[i]
        }
    }
    return sum;
}
