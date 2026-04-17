void s453(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < iterations * 2; nl++) {
        for (int i = 0; i < LEN_1D; i++) {
            a[i] = (float)(2 * (i + 1)) * b[i];
        }
    }
}