void s453_opt(int iterations, int LEN_1D, float* a, float *b) {
    for (int nl = 0; nl < iterations * 2; nl++) {
        for (int i = 0; i < LEN_1D; i++) {
            float s = (i + 1) * 2.0f;
            a[i] = s * b[i];
        }
    }
}
