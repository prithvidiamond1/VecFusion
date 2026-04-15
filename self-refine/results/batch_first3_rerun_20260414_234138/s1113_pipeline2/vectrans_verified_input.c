void s1113_opt(int iterations, int LEN_1D, float* a, float* b)
{
    int mid = LEN_1D / 2;
    for (int nl = 0; nl < 2*iterations; nl++) {
        float mid_val = a[mid];
        for (int i = 0; i < mid; i++) {
            a[i] = mid_val + b[i];
        }
        a[mid] = mid_val + b[mid];
        float new_mid_val = a[mid];
        for (int i = mid + 1; i < LEN_1D; i++) {
            a[i] = new_mid_val + b[i];
        }
    }
}