void s256(int iterations, float* a, float* d, float aa[256][256], float bb[256][256])
{
    int loop_count = 10 * (iterations / 256);
    
    for (int nl = 0; nl < loop_count; nl++) {
        // Precompute all a[j] values using closed-form solution
        float a0 = a[0];
        float a_temp[256];
        a_temp[0] = a0;
        
        // Vectorizable loop: compute a[j] = (j % 2 == 1) ? (1.0f - a0) : a0
        for (int j = 1; j < 256; j++) {
            a_temp[j] = (j & 1) ? (1.0f - a0) : a0;
        }
        
        // Update original a array
        for (int j = 1; j < 256; j++) {
            a[j] = a_temp[j];
        }
        
        // Compute aa[j][i] with i-loop outermost for better locality
        for (int i = 0; i < 256; i++) {
            for (int j = 1; j < 256; j++) {
                aa[j][i] = a_temp[j] + bb[j][i] * d[j];
            }
        }
    }
}