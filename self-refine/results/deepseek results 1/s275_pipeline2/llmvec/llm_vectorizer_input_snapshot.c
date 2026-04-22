void s275(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    int loop_count = 10 * (iterations / 256);
    
    for (int nl = 0; nl < loop_count; nl++) {
        // Precompute condition mask for all columns
        int cond[256];
        for (int i = 0; i < 256; i++) {
            cond[i] = aa[0][i] > 0.f;
        }
        
        // Process each row sequentially (dependency in j direction)
        for (int j = 1; j < 256; j++) {
            // Vector-friendly loop across columns
            for (int i = 0; i < 256; i++) {
                if (cond[i]) {
                    aa[j][i] = aa[j-1][i] + bb[j][i] * cc[j][i];
                }
            }
        }
    }
}