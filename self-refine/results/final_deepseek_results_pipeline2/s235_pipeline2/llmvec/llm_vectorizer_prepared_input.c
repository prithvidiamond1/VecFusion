void s235(int iterations, float* a, float* b, float* c, float aa[256][256], float bb[256][256])
{
    for (int nl = 0; nl < 200*(iterations/256); nl++) {
        // Vectorizable update of a[i] - completely independent
        for (int i = 0; i < 256; i++) {
            a[i] += b[i] * c[i];
        }
        
        // Process each column independently - enable potential vectorization across i
        for (int i = 0; i < 256; i++) {
            float ai = a[i];  // Capture updated a[i] once
            float aa_prev = aa[0][i];  // Initial value for recurrence
            
            // Process recurrence sequentially - dependency chain remains
            for (int j = 1; j < 256; j++) {
                aa_prev = aa_prev + bb[j][i] * ai;
                aa[j][i] = aa_prev;
            }
        }
    }
}
