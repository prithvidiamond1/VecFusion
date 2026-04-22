float vectorized_s352(int iterations, int LEN_1D, float* a, float *b) {
    float dot = 0.0f;
    
    for (int nl = 0; nl < 8 * iterations; nl++) {
        float partial_dot = 0.0f;
        
        // Process all elements sequentially in a single loop
        for (int i = 0; i < LEN_1D; i++) {
            partial_dot += a[i] * b[i];
        }
        
        dot = partial_dot;
    }
    
    return dot;
}
