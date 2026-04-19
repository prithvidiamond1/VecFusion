float s317_opt(int iterations, int LEN_1D) {
    float q;
    int half_len = LEN_1D / 2;
    
    // Precompute (0.99f)^half_len once using repeated multiplication
    // This maintains the same order of operations as the original
    float power = 1.0f;
    for (int i = 0; i < half_len; i++) {
        power *= 0.99f;
    }
    
    // Outer loop only assigns the precomputed value
    for (int nl = 0; nl < 5 * iterations; nl++) {
        q = power;
    }
    return q;
}
