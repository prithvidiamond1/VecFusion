void s232(int iterations, float aa[256][256], float bb[256][256])
{
    for (int nl = 0; nl < 100*(iterations/(256)); nl++) {
        for (int j = 1; j < 256; j++) {
            float prev = aa[j][0];
            for (int i = 1; i <= j; i++) {
                float temp = prev * prev + bb[j][i];
                prev = temp;
                aa[j][i] = temp;
            }
        }
    }
}