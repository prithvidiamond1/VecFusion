
void s231_opt(int iterations, float aa[256][256], float bb[256][256]) {
    for (int nl = 0; nl < 100*(iterations/256); nl++) {
        for (int i = 0; i < 256; ++i) {
            float prev = aa[0][i];
            for (int j = 1; j < 256; j++) {
                float temp = prev + bb[j][i];
                prev = temp;
                aa[j][i] = temp;
            }
        }
    }
}
