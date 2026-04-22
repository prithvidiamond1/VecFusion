void s275(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    int nl_iterations = 10 * (iterations / 256);
    
    for (int nl = 0; nl < nl_iterations; nl++) {
        // Store initial values of aa[0][i] before any modifications
        float init_aa0[256];
        for (int i = 0; i < 256; i++) {
            init_aa0[i] = aa[0][i];
        }
        
        // Process each column independently
        for (int i = 0; i < 256; i++) {
            if (init_aa0[i] > 0.0f) {
                float prev = init_aa0[i];
                // Process with explicit dependency chain
                for (int j = 1; j < 256; j++) {
                    prev = prev + bb[j][i] * cc[j][i];
                    aa[j][i] = prev;
                }
            }
        }
    }
}
