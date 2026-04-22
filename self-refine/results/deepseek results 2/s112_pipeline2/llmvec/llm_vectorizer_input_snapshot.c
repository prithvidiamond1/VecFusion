void s112(float *a, float *b, int iterations, int LEN_1D) {
    float *temp = (float*)malloc(LEN_1D * sizeof(float));
    
    for (int nl = 0; nl < 3 * iterations; nl++) {
        // Copy original 'a' values to temp before modification
        for (int i = 0; i < LEN_1D; i++) {
            temp[i] = a[i];
        }
        
        // Process all iterations in forward order using original values
        for (int i = 0; i < LEN_1D - 1; i++) {
            a[i + 1] = temp[i] + b[i];
        }
    }
    
    free(temp);
}