void s141_opt(int iterations, float * flat_2d_array, float bb[256][256])
{
    int nl_max = 200 * (iterations / 256);
    
    // Precompute k indices once
    int k_indices[256][256];
    for (int i = 0; i < 256; i++) {
        int k = i * (i + 3) / 2;
        for (int j = i; j < 256; j++) {
            k_indices[i][j] = k;
            k += j + 1;
        }
    }
    
    // Process outer loop
    for (int nl = 0; nl < nl_max; nl++) {
        // Loop distribution with temporary accumulation
        for (int i = 0; i < 256; i++) {
            float temp_accum[256 - i];
            
            // First: gather all bb values (fully vectorizable)
            for (int j = i; j < 256; j++) {
                temp_accum[j - i] = bb[j][i];
            }
            
            // Second: accumulate using precomputed indices
            // Use local pointer for better aliasing analysis
            float *local_flat = flat_2d_array;
            const int *local_k_idx = &k_indices[i][i];
            for (int j = 0; j < 256 - i; j++) {
                local_flat[local_k_idx[j]] += temp_accum[j];
            }
        }
    }
}
