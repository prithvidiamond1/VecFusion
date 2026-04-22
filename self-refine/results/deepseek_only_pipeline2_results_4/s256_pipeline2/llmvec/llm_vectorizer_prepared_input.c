void s256(int iterations, float* a, float* d, float aa[256][256], float bb[256][256]) {
    for (int nl = 0; nl < 10 * (iterations / 256); nl++) {
        float a0 = a[0];
        
        for (int i = 0; i < 256; i++) {
            // Compute a[j] values for this i iteration using recurrence
            float a_prev = (i == 0) ? a0 : a[0];  // Start from appropriate value
            for (int j = 1; j < 256; j++) {
                a_prev = (float)1.0 - a_prev;
                a[j] = a_prev;
            }
            
            // Compute aa[j][i] using updated a[j] values
            for (int j = 1; j < 256; j++) {
                aa[j][i] = a[j] + bb[j][i] * d[j];
            }
        }
    }
}