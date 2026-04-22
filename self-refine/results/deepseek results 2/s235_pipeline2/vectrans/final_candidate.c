void s235_opt(int iterations, float* a, float* b, float* c, float aa[256][256], float bb[256][256]) {
    for (int nl = 0; nl < 200 * (iterations / 256); nl++) {
        // Loop distribution: separate a[i] update from aa computation
        for (int i = 0; i < 256; i++) {
            a[i] += b[i] * c[i];
        }
        
        // Loop reordering: j outer, i inner with tiling for cache locality
        const int TILE_SIZE = 64;
        for (int i0 = 0; i0 < 256; i0 += TILE_SIZE) {
            int i_end = i0 + TILE_SIZE;
            if (i_end > 256) i_end = 256;
            
            for (int j = 1; j < 256; j++) {
                for (int i = i0; i < i_end; i++) {
                    aa[j][i] = aa[j-1][i] + bb[j][i] * a[i];
                }
            }
        }
    }
}
