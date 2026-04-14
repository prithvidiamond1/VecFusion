void s1113_opt(int iterations, int LEN_1D, float* a, float* b)
{
    int mid = LEN_1D / 2;
    for (int nl = 0; nl < 2*iterations; nl++) {
        float amid = a[mid];
        for (int i = 0; i < mid; i++) {
            a[i] = amid + b[i];
        }
        a[mid] = amid + b[mid];
        float amid_new = a[mid];
        for (int i = mid + 1; i < LEN_1D; i++) {
            a[i] = amid_new + b[i];
        }
    }
}
