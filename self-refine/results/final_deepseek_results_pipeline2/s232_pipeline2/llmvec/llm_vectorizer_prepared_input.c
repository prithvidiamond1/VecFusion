void s232(int iterations, float aa[256][256], float bb[256][256])
{
    int loop_count = 100 * (iterations / 256);
    
    for (int nl = 0; nl < loop_count; nl++) {
        for (int j = 1; j < 256; j++) {
            // Store the previous value to break dependency chain
            float prev = aa[j][0];
            
            for (int i = 1; i <= j; i++) {
                // Compute using stored previous value
                float current = prev * prev + bb[j][i];
                // Update array and store for next iteration
                aa[j][i] = current;
                prev = current;
            }
        }
    }
}
