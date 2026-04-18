float s314(int iterations, int LEN_1D, float* a) {
    float x;
    for (int nl = 0; nl < iterations*5; nl++) {
        float max_val = a[0];
        int i;
        
        // Process in chunks to allow potential vectorization
        for (i = 1; i < LEN_1D; i++) {
            float current = a[i];
            if (current > max_val) {
                max_val = current;
            }
        }
        
        x = max_val;
    }
    return x;
}