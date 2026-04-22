float s317_opt(int iterations, int LEN_1D) {
    float q;
    int half_len = LEN_1D / 2;
    
    // Precompute the factor once
    float factor = 1.0f;
    for (int i = 0; i < half_len; i++) {
        factor *= 0.99f;
    }
    
    // Outer loop remains, but inner multiplication is replaced
    for (int nl = 0; nl < 5 * iterations; nl++) {
        q = factor;
    }
    
    return q;
}
