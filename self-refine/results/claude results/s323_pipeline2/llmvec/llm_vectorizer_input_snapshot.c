void s323(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        float b_prev = b[0];
        for (int i = 1; i < LEN_1D; i++) {
            float ai = b_prev + c[i] * d[i];
            float bi = ai + c[i] * e[i];
            a[i] = ai;
            b[i] = bi;
            b_prev = bi;
        }
    }
}