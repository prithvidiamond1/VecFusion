
void s2233_opt(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    int loop_count = 100 * (iterations / 256);
    
    for (int nl = 0; nl < loop_count; nl++) {
        // Process aa updates with loop interchange for vectorization
        for (int j = 1; j < 256; j++) {
            for (int i = 1; i < 256; i++) {
                aa[j][i] = aa[j-1][i] + cc[j][i];
            }
        }
        
        // Process bb updates - row-wise recurrence
        for (int i = 1; i < 256; i++) {
            for (int j = 1; j < 256; j++) {
                bb[i][j] = bb[i-1][j] + cc[i][j];
            }
        }
    }
}
