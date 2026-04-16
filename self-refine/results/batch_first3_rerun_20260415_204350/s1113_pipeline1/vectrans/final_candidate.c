void s1113_opt(int iterations, int LEN_1D, float* a, float* b)
{
    int mid = LEN_1D / 2;
    for (int nl = 0; nl < 2*iterations; nl++) {
        float temp = a[mid];
        for (int i = 0; i < mid; i++) {
            a[i] = temp + b[i];
        }
        a[mid] = temp + b[mid];
        float temp2 = a[mid];
        for (int i = mid + 1; i < LEN_1D; i++) {
            a[i] = temp2 + b[i];
        }
    }
}
