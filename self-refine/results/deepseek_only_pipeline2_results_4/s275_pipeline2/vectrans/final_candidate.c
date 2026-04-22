void s275_opt(int iterations, float aa[256][256], float bb[256][256], float cc[256][256]) {
    for (int nl = 0; nl < 10 * (iterations / 256); nl++) {
        // Create mask for columns where aa[0][i] > 0
        int mask[256];
        for (int i = 0; i < 256; i++) {
            mask[i] = (aa[0][i] > (float)0.) ? 1 : 0;
        }
        
        // Process columns with mask-based computation
        for (int j = 1; j < 256; j++) {
            for (int i = 0; i < 256; i++) {
                if (mask[i]) {
                    aa[j][i] = aa[j-1][i] + bb[j][i] * cc[j][i];
                }
            }
        }
    }
}
