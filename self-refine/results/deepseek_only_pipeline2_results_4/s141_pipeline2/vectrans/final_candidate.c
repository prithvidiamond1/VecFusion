void s141_opt(int iterations, float * flat_2d_array, float bb[256][256])
{
    int nl_iterations = 200 * (iterations / 256);
    
    for (int nl = 0; nl < nl_iterations; nl++) {
        for (int j = 0; j < 256; j++) {
            int base_k = j * (j + 1) / 2;
            for (int i = 0; i <= j; i++) {
                flat_2d_array[base_k + i] += bb[j][i];
            }
        }
    }
}
