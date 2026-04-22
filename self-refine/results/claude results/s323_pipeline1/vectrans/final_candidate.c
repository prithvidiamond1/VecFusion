void s323_opt(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    float b_orig[LEN_1D];
    for (int nl = 0; nl < iterations/2; nl++) {
        memcpy(b_orig, b, LEN_1D * sizeof(float));
        for (int i = 1; i < LEN_1D; i++) {
            b[i] = b[i-1] + c[i] * (d[i] + e[i]);
        }
        for (int i = 1; i < LEN_1D; i++) {
            a[i] = b_orig[i-1] + c[i] * d[i];
        }
    }
}
