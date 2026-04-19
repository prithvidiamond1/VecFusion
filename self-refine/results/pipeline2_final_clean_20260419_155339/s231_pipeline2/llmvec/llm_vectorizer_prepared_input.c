void s231(int iterations, float aa[256][256], float bb[256][256])
{
    int loop_count = 100 * (iterations / 256);
    
    for (int nl = 0; nl < loop_count; nl++) {
        // Process each column independently - enables vectorization across columns
        for (int j = 1; j < 256; j++) {
            for (int i = 0; i < 256; i++) {
                aa[j][i] = aa[j - 1][i] + bb[j][i];
            }
        }
    }
}