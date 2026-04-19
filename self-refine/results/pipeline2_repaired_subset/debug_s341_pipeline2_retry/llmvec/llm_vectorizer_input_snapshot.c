void s341(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < iterations; nl++) {
        int j = -1;
        // First loop: compute mask and prefix sum for write indices
        int mask[LEN_1D];
        int write_idx[LEN_1D];
        
        // Vectorizable: compute mask and prefix sum
        for (int i = 0; i < LEN_1D; i++) {
            mask[i] = (b[i] > (float)0.);
        }
        
        // Prefix sum computation
        for (int i = 0; i < LEN_1D; i++) {
            j += mask[i];
            write_idx[i] = j;
        }
        
        // Vectorizable: scatter positive values to 'a'
        for (int i = 0; i < LEN_1D; i++) {
            if (mask[i]) {
                a[write_idx[i]] = b[i];
            }
        }
    }
}