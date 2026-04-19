void s141(int iterations, float * flat_2d_array, float bb[256][256]) {
    int nl_iter = 200 * (iterations / 256);
    
    for (int nl = 0; nl < nl_iter; nl++) {
        // First: accumulate bb values into flat_2d_array with regular access pattern
        for (int i = 0; i < 256; i++) {
            float accum[256];
            // Vectorizable accumulation: regular access to bb[j][i]
            for (int j = i; j < 256; j++) {
                accum[j] = bb[j][i];
            }
            // Scatter accumulated values to flat_2d_array
            int k = (i+1) * i / 2 + i;
            for (int j = i; j < 256; j++) {
                flat_2d_array[k] += accum[j];
                k += j + 1;
            }
        }
    }
}