void s232(int iterations, float aa[256][256], float bb[256][256])
{
    for (int nl = 0; nl < 100*(iterations/(256)); nl++) {
        for (int i = 1; i < 256; i++) {
            for (int j = i; j < 256; j++) {
                aa[j][i] = aa[j][i-1]*aa[j][i-1] + bb[j][i];
            }
        }
    }
}