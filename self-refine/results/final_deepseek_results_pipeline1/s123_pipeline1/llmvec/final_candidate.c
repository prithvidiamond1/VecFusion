void vectorized_s123(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    int half_len = LEN_1D / 2;
    for (int nl = 0; nl < iterations; nl++) {
        int j = -1;
        for (int i = 0; i < half_len; i++) {
            j++;
            a[j] = b[i] + d[i] * e[i];
            if (c[i] > 0.0f) {
                j++;
                a[j] = c[i] + d[i] * e[i];
            }
        }
    }
}
