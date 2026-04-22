void s114(int iterations, float aa[256][256], float bb[256][256])
{
    float trans[256][256];
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < i; j++) {
            trans[i][j] = aa[j][i];
        }
    }

    for (int nl = 0; nl < 200*(iterations/(256)); nl++) {
        for (int i = 1; i < 256; i++) {
            for (int j = 0; j < i; j++) {
                aa[i][j] = trans[i][j] + bb[i][j];
            }
        }
        for (int i = 0; i < 256; i++) {
            for (int j = 0; j < i; j++) {
                trans[i][j] = aa[j][i];
            }
        }
    }
}