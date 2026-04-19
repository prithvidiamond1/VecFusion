void s141(int iterations, float * flat_2d_array, float bb[256][256])
{
    int nl_iterations = 200 * (iterations / 256);
    
    for (int nl = 0; nl < nl_iterations; nl++) {
        // Loop distribution: Separate the accumulation into two phases
        // First, compute all k indices and accumulate contributions for each i
        for (int i = 0; i < 256; i++) {
            // Precompute starting index
            int k_start = (i+1) * i / 2 + i;
            
            // Instruction reordering: Compute all k values first
            int k_indices[256];
            int k = k_start;
            for (int j = i; j < 256; j++) {
                k_indices[j] = k;
                k += j + 1;
            }
            
            // Loop splitting: Separate the read from bb and write to flat_2d_array
            // This creates two independent loops that can be optimized separately
            
            // First loop: Read from bb (contiguous access, can be vectorized)
            float temp_vals[256];
            for (int j = i; j < 256; j++) {
                temp_vals[j] = bb[j][i];
            }
            
            // Second loop: Write to flat_2d_array using precomputed indices
            // This still has non-linear access but is now independent
            for (int j = i; j < 256; j++) {
                flat_2d_array[k_indices[j]] += temp_vals[j];
            }
        }
    }
}