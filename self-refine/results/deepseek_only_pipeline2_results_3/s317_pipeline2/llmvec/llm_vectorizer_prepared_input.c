float s317(int iterations, int LEN_1D) {
    float q;
    int half_len = LEN_1D / 2;
    float factor = (float)0.99;
    
    for (int nl = 0; nl < 5 * iterations; nl++) {
        q = (float)1.;
        float temp = q;
        
        for (int i = 0; i < half_len; i++) {
            temp *= factor;
        }
        
        q = temp;
    }
    return q;
}