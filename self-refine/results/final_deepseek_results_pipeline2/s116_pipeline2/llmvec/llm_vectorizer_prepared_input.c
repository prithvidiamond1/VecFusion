void s116(int iterations, int LEN_1D, float* a) {
    for (int nl = 0; nl < iterations * 10; nl++) {
        // Loop distribution: process each position in the 5-element pattern separately
        // This creates uniform operations within each loop for better vectorization
        
        // Position 0: a[i] = a[i+1] * a[i]
        for (int i = 0; i < LEN_1D - 5; i += 5) {
            a[i] = a[i + 1] * a[i];
        }
        
        // Position 1: a[i+1] = a[i+2] * a[i+1]
        for (int i = 0; i < LEN_1D - 5; i += 5) {
            a[i + 1] = a[i + 2] * a[i + 1];
        }
        
        // Position 2: a[i+2] = a[i+3] * a[i+2]
        for (int i = 0; i < LEN_1D - 5; i += 5) {
            a[i + 2] = a[i + 3] * a[i + 2];
        }
        
        // Position 3: a[i+3] = a[i+4] * a[i+3]
        for (int i = 0; i < LEN_1D - 5; i += 5) {
            a[i + 3] = a[i + 4] * a[i + 3];
        }
        
        // Position 4: a[i+4] = a[i+5] * a[i+4]
        for (int i = 0; i < LEN_1D - 5; i += 5) {
            a[i + 4] = a[i + 5] * a[i + 4];
        }
    }
}
