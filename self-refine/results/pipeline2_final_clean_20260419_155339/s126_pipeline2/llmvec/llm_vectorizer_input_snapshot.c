void s126(int iterations, float bb[256][256], float cc[256][256], float * flat_2d_array)
{
    for (int nl = 0; nl < 10*(iterations/256); nl++) {
        int k = 1;
        
        // Process each column independently
        for (int i = 0; i < 256; i++) {
            // Store the initial value for this column
            float prev_bb = bb[0][i];
            
            // Precompute all k indices for this column
            int k_indices[256];
            for (int j = 1; j < 256; j++) {
                k_indices[j] = k - 1;
                k++;
            }
            k++; // Extra increment after column
            
            // First: compute all the multiplication results
            float mul_results[256];
            for (int j = 1; j < 256; j++) {
                mul_results[j] = flat_2d_array[k_indices[j]] * cc[j][i];
            }
            
            // Second: perform the cumulative sum (serial dependency)
            for (int j = 1; j < 256; j++) {
                prev_bb = prev_bb + mul_results[j];
                bb[j][i] = prev_bb;
            }
        }
    }
}