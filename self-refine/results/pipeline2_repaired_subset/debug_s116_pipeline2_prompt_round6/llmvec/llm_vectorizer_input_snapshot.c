void s116(int iterations, int LEN_1D, float* a) {
    for (int nl = 0; nl < iterations * 10; nl++) {
        // Process all elements except the last 5 in a single stride-1 loop
        // This allows vectorization since each iteration only uses original values
        for (int i = 0; i < LEN_1D - 5; i++) {
            a[i] = a[i + 1] * a[i];
        }
        
        // Handle the remaining 5 elements with their specific update pattern
        // This small loop won't significantly impact performance
        for (int i = LEN_1D - 5; i < LEN_1D - 4; i++) {
            float t1 = a[i + 1];
            float t2 = a[i + 2];
            float t3 = a[i + 3];
            float t4 = a[i + 4];
            float t5 = a[i + 5];
            
            a[i] = t1 * a[i];
            a[i + 1] = t2 * t1;
            a[i + 2] = t3 * t2;
            a[i + 3] = t4 * t3;
            a[i + 4] = t5 * t4;
        }
    }
}