
void s233_opt(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    for (int nl = 0; nl < 100*(iterations/256); nl++) {
        // Process aa and bb separately to remove inter-loop dependencies
        for (int i = 1; i < 256; i++) {
            // aa loop: j-direction dependency, keep j as inner loop
            for (int j = 1; j < 256; j++) {
                aa[j][i] = aa[j-1][i] + cc[j][i];
            }
        }
        
        for (int i = 1; i < 256; i++) {
            // bb loop: i-direction dependency, keep i as outer loop
            for (int j = 1; j < 256; j++) {
                bb[j][i] = bb[j][i-1] + cc[j][i];
            }
        }
    }
}
