void s1113_opt(int iterations, int LEN_1D, float* a, float* b)
{
    int mid = LEN_1D / 2;
    for (int nl = 0; nl < 2 * iterations; nl++) {
        // Phase 1: i = 0 to mid-1, a[mid] not yet updated
        float mid_val = a[mid];
        for (int i = 0; i < mid; i++) {
            a[i] = mid_val + b[i];
        }
        // Mid-point update
        a[mid] = mid_val + b[mid];
        // Phase 2: i = mid+1 to LEN_1D-1, use updated a[mid]
        float new_mid_val = a[mid];
        for (int i = mid + 1; i < LEN_1D; i++) {
            a[i] = new_mid_val + b[i];
        }
    }
}
