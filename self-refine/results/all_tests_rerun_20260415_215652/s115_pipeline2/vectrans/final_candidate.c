void s115_opt(int iterations, float* a, float aa[256][256])
{
    for (int nl = 0; nl < 1000*(iterations/256); nl++) {
        for (int j = 0; j < 256; j++) {
            float aj = a[j];
            for (int i = j+1; i < 256; i++) {
                a[i] -= aa[j][i] * aj;
            }
        }
    }
}
