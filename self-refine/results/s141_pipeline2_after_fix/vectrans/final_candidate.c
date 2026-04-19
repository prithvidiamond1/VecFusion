void s141_opt(int iterations, float * flat_2d_array, float bb[256][256])
{
    for (int nl = 0; nl < 200*(iterations/256); nl++) {
        // Pre-compute all k indices for the triangular pattern
        int k_indices[256][256];
        for (int i = 0; i < 256; i++) {
            int k = (i+1) * i / 2 + i;  // k_start for this i
            for (int j = i; j < 256; j++) {
                k_indices[i][j] = k;
                k += j+1;
            }
        }
        
        // Now we can process in a more vector-friendly way
        // Process by column of bb (fixed i, varying j)
        for (int i = 0; i < 256; i++) {
            // Create a temporary array to hold the values to add
            float temp_values[256];
            for (int j = i; j < 256; j++) {
                temp_values[j] = bb[j][i];
            }
            
            // Now add each value to its corresponding position
            for (int j = i; j < 256; j++) {
                flat_2d_array[k_indices[i][j]] += temp_values[j];
            }
        }
    }
}
