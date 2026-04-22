void s233(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    for (int nl = 0; nl < 100*(iterations/256); nl++) {
        // Process bb: horizontal recurrence - vectorizable along j dimension
        for (int i = 1; i < 256; i++) {
            for (int j = 1; j < 256; j++) {
                bb[j][i] = bb[j][i-1] + cc[j][i];
            }
        }
        
        // Process aa: vertical recurrence - use temporary array to break dependency
        for (int i = 1; i < 256; i++) {
            float temp[256];
            // Initialize temp[0] with aa[0][i]
            temp[0] = aa[0][i];
            
            // Compute prefix sum in temp array - vectorizable
            for (int j = 1; j < 256; j++) {
                temp[j] = temp[j-1] + cc[j][i];
            }
            
            // Copy results back to aa - vectorizable
            for (int j = 1; j < 256; j++) {
                aa[j][i] = temp[j];
            }
        }
    }
}