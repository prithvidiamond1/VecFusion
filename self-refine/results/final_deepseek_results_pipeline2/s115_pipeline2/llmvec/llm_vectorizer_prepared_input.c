void s115(int iterations, float* a, float aa[256][256])
{
    int loop_count = 1000 * (iterations / 256);
    
    for (int nl = 0; nl < loop_count; nl++) {
        for (int j = 0; j < 256; j++) {
            float aj = a[j];
            float* aa_j = aa[j];
            
            for (int i = j + 1; i < 256; i++) {
                a[i] -= aa_j[i] * aj;
            }
        }
    }
}
