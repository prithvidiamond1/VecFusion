void s233(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    int loop_count = 100 * (iterations / 256);
    
    for (int nl = 0; nl < loop_count; nl++) {
        // Process aa updates - column-wise dependency along j
        for (int i = 1; i < 256; i++) {
            float prev = aa[0][i];  // Store first element for dependency chain
            for (int j = 1; j < 256; j++) {
                float temp = prev + cc[j][i];
                prev = temp;
                aa[j][i] = temp;
            }
        }
        
        // Process bb updates - row-wise dependency along i
        // Can be vectorized over j dimension
        for (int j = 1; j < 256; j++) {
            float prev = bb[j][0];  // Store first element for each row
            for (int i = 1; i < 256; i++) {
                float temp = prev + cc[j][i];
                prev = temp;
                bb[j][i] = temp;
            }
        }
    }
}