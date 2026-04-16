void s211_opt(int iterations, int LEN_1D, float*a, float*b,float*c,float*d,float*e)
{
    float *b_orig = (float*)malloc(LEN_1D * sizeof(float));
    for (int nl = 0; nl < iterations; nl++) {
        memcpy(b_orig, b, LEN_1D * sizeof(float));
        for (int i = 1; i < LEN_1D-1; i++) {
            b[i] = b_orig[i + 1] - e[i] * d[i];
        }
        for (int i = 1; i < LEN_1D-1; i++) {
            a[i] = b[i - 1] + c[i] * d[i];
        }
    }
    free(b_orig);
}
