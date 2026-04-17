void s343_opt(int iterations, float *flat_2d_array, float aa[256][256], float bb[256][256])
{
    int outer_loops = 10 * (iterations / 256);
    
    for (int nl = 0; nl < outer_loops; nl++) {
        int k = -1;
        
        // Process in column-major order (i outer, j inner)
        for (int i = 0; i < 256; i++) {
            // First, compute mask and prefix sum for this column
            int mask[256];
            int prefix_sum[256];
            int count = 0;
            
            // Vector-friendly condition check (no branching)
            for (int j = 0; j < 256; j++) {
                mask[j] = (bb[j][i] > 0.0f);
                count += mask[j];
                prefix_sum[j] = count;
            }
            
            // Then write the selected elements sequentially
            for (int j = 0; j < 256; j++) {
                if (mask[j]) {
                    int write_idx = k + prefix_sum[j];
                    flat_2d_array[write_idx] = aa[j][i];
                }
            }
            
            k += count;
        }
    }
}
