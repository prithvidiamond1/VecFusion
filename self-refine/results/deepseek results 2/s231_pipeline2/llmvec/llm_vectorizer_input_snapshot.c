void s231(int iterations, float aa[256][256], float bb[256][256]) {
    int loop_count = 100 * (iterations / 256);
    for (int nl = 0; nl < loop_count; nl++) {
        for (int j = 1; j < 256; j++) {
            for (int i = 0; i < 256; i += 4) {
                // Vector-friendly: process 4 i-elements at a time
                float prev0 = aa[j-1][i];
                float prev1 = aa[j-1][i+1];
                float prev2 = aa[j-1][i+2];
                float prev3 = aa[j-1][i+3];
                
                aa[j][i]   = prev0 + bb[j][i];
                aa[j][i+1] = prev1 + bb[j][i+1];
                aa[j][i+2] = prev2 + bb[j][i+2];
                aa[j][i+3] = prev3 + bb[j][i+3];
            }
        }
    }
}