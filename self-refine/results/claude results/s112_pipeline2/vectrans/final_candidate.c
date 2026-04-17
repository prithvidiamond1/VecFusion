void s112_opt(float *a, float *b, int iterations, int LEN_1D) {
    float *tmp = (float *)malloc(LEN_1D * sizeof(float));
    for (int nl = 0; nl < 3 * iterations; nl++) {
        memcpy(tmp, a, LEN_1D * sizeof(float));
        for (int i = 0; i <= LEN_1D - 2; i++) {
            a[i + 1] = tmp[i] + b[i];
        }
    }
    free(tmp);
}
