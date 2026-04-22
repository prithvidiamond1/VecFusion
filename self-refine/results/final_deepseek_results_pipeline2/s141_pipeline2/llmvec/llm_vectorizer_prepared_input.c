void s141(int iterations, float * flat_2d_array, float bb[256][256]) {
    int nl_iter = 200 * (iterations / 256);
    
    for (int nl = 0; nl < nl_iter; nl++) {
        // Precompute all k values for each i (starting point for each row)
        int k_start[256];
        for (int i = 0; i < 256; i++) {
            k_start[i] = (i+1) * i / 2 + i;
        }
        
        // Process by j first to enable vectorization on i
        // This creates regular access pattern: bb[j][i] for i=0..j
        for (int j = 0; j < 256; j++) {
            // For each j, process all i <= j
            // Compute k incrementally for each i
            int k = k_start[0] + j;  // k for i=0, j=j
            for (int i = 0; i <= j; i++) {
                flat_2d_array[k] += bb[j][i];
                // Update k for next i: k(i+1,j) = k(i,j) + (j+1) - (i+1)
                if (i < j) {
                    k += (j - i);
                }
            }
        }
    }
}
