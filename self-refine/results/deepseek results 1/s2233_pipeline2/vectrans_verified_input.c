void s2233(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    int loop_count = 100 * (iterations / 256);
    
    for (int nl = 0; nl < loop_count; nl++) {
        // Process bb updates first to maintain dependency chain
        for (int i = 1; i < 256; i++) {
            // Row i of bb: sequential recurrence in j direction
            for (int j = 1; j < 256; j++) {
                bb[i][j] = bb[i-1][j] + cc[i][j];
            }
        }
        
        // Process aa updates column by column
        for (int i = 1; i < 256; i++) {
            // Column i of aa: sequential recurrence in j direction
            for (int j = 1; j < 256; j++) {
                aa[j][i] = aa[j-1][i] + cc[j][i];
            }
        }
    }
}