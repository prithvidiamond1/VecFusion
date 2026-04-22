
void s114_opt(int iterations, float aa[256][256], float bb[256][256]) {
    int loop_count = 200 * (iterations / 256);
    
    for (int nl = 0; nl < loop_count; nl++) {
        // Copy entire matrix to temporary storage
        float temp[256][256];
        for (int i = 0; i < 256; i++) {
            for (int j = 0; j < 256; j++) {
                temp[i][j] = aa[i][j];
            }
        }
        
        // Process lower triangle with independent reads from temp
        for (int i = 0; i < 256; i++) {
            for (int j = 0; j < i; j++) {
                aa[i][j] = temp[j][i] + bb[i][j];
            }
        }
    }
}
