void s141(int iterations, float * flat_2d_array, float bb[256][256])
{
    int nl_iter = 200 * (iterations / 256);
    
    // Precompute all k indices for each (i,j) pair
    int k_indices[256][256];
    for (int i = 0; i < 256; i++) {
        int k = (i+1) * i / 2 + i;
        for (int j = i; j < 256; j++) {
            k_indices[i][j] = k;
            k += j + 1;
        }
    }
    
    // Loop distribution: separate accumulation from index calculation
    for (int nl = 0; nl < nl_iter; nl++) {
        // First loop: accumulate contributions in a vector-friendly way
        for (int i = 0; i < 256; i++) {
            // Temporary array for contiguous accumulation
            float temp_accum[256];
            
            // Vectorizable accumulation of bb values - use memcpy for column access
            // Load bb column i into temp_accum starting at position i
            for (int j = i; j < 256; j++) {
                temp_accum[j] = bb[j][i];
            }
            
            // Scatter accumulated values to flat_2d_array
            // This loop has indirect addressing but is independent of the first loop
            for (int j = i; j < 256; j++) {
                int k = k_indices[i][j];
                flat_2d_array[k] += temp_accum[j];
            }
        }
    }
}