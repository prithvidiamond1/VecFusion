void s141_opt(int iterations, float * flat_2d_array, float bb[256][256])
{
    int nl_iterations = 200 * (iterations / 256);
    
    for (int nl = 0; nl < nl_iterations; nl++) {
        // Loop interchange: j outer, i inner for stride-1 access
        for (int j = 0; j < 256; j++) {
            // Precompute base index for this j
            int base_idx = j * (j + 1) / 2;
            
            // Vectorizable inner loop: i from 0 to j
            for (int i = 0; i <= j; i++) {
                flat_2d_array[base_idx + i] += bb[j][i];
            }
        }
    }
}
