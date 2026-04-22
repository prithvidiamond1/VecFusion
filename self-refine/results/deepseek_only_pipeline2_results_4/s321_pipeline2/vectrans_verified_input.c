void s321(int iterations, int LEN_1D, float* a, float* b) {
    for (int nl = 0; nl < iterations; nl++) {
        // Store original values to break dependency
        float* temp = (float*)malloc(LEN_1D * sizeof(float));
        memcpy(temp, a, LEN_1D * sizeof(float));
        
        float prev = a[0];
        for (int i = 1; i < LEN_1D; i++) {
            a[i] = temp[i] + prev * b[i];
            prev = a[i];
        }
        
        free(temp);
    }
}