
void s232_opt(int iterations, float aa[256][256], float bb[256][256])
{
    int loop_count = 100 * (iterations / 256);
    
    for (int nl = 0; nl < loop_count; nl++) {
        // Loop interchange: make j the inner loop for vectorization
        for (int i = 1; i < 256; i++) {
            // Process columns where j >= i (triangular region)
            for (int j = i; j < 256; j++) {
                // Each column j has its own recurrence chain
                aa[j][i] = aa[j][i-1] * aa[j][i-1] + bb[j][i];
            }
        }
    }
}
