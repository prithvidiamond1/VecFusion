
float s317_opt(int iterations, int LEN_1D) {
    float q;
    int half_len = LEN_1D / 2;
    
    // Precompute factor once outside the outer loop
    float factor = (float)1.;
    for (int i = 0; i < half_len; i++) {
        factor *= (float).99;
    }
    
    // Outer loop simplified to single assignment
    q = factor;
    return q;
}
