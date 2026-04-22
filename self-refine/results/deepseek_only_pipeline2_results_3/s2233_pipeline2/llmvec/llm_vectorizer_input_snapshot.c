void s2233(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    for (int nl = 0; nl < 100*(iterations/256); nl++) {
        // Process aa and bb updates in the original order (i outer loop)
        for (int i = 1; i < 256; i++) {
            // aa update: dependency on aa[j-1][i] prevents vectorization in j
            // Keep as is for semantic correctness
            for (int j = 1; j < 256; j++) {
                aa[j][i] = aa[j-1][i] + cc[j][i];
            }
            
            // bb update: dependency on bb[i-1][j] is loop-invariant for inner j loop
            // This inner loop can be vectorized in j dimension
            for (int j = 1; j < 256; j++) {
                bb[i][j] = bb[i-1][j] + cc[i][j];
            }
        }
    }
}