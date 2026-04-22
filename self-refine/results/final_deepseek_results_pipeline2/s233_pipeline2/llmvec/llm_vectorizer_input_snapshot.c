void s233_opt(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    int nl_iterations = 100 * (iterations / 256);
    
    for (int nl = 0; nl < nl_iterations; nl++) {
        // Process aa with j outer, i inner for vectorization
        for (int j = 1; j < 256; j++) {
            for (int i = 1; i < 256; i++) {
                aa[j][i] = aa[j-1][i] + cc[j][i];
            }
        }
        
        // Process bb with i outer, j inner to expose vectorization
        for (int i = 1; i < 256; i++) {
            for (int j = 1; j < 256; j++) {
                bb[j][i] = bb[j][i-1] + cc[j][i];
            }
        }
    }
}
