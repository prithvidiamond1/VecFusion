void s235(int iterations, float* a, float* b, float* c, float aa[256][256], float bb[256][256])
{
    int total_nl = 200 * (iterations / 256);
    
    for (int nl = 0; nl < total_nl; nl++) {
        // First: compute all a[i] updates
        for (int i = 0; i < 256; i++) {
            a[i] += b[i] * c[i];
        }
        
        // Second: compute aa columns independently
        for (int i = 0; i < 256; i++) {
            float a_val = a[i];  // Store in register for reuse
            float aa_prev = aa[0][i];  // Store previous value
            
            for (int j = 1; j < 256; j++) {
                aa_prev = aa_prev + bb[j][i] * a_val;
                aa[j][i] = aa_prev;
            }
        }
    }
}