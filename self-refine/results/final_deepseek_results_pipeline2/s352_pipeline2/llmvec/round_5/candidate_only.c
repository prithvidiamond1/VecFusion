float vectorized_s352(int iterations, int LEN_1D, float* a, float *b) {
    float dot = 0.0f;
    
    for (int nl = 0; nl < 8 * iterations; nl++) {
        float partial_dot = 0.0f;
        
        // Process in groups of 5 to match original accumulation pattern
        int i = 0;
        for (; i + 4 < LEN_1D; i += 5) {
            // Preserve exact accumulation order
            partial_dot += a[i] * b[i];
            partial_dot += a[i + 1] * b[i + 1];
            partial_dot += a[i + 2] * b[i + 2];
            partial_dot += a[i + 3] * b[i + 3];
            partial_dot += a[i + 4] * b[i + 4];
        }
        
        // Handle remaining elements (0-4)
        for (; i < LEN_1D; i++) {
            partial_dot += a[i] * b[i];
        }
        
        dot = partial_dot;  // Overwrite dot each iteration like original
    }
    
    return dot;
}
