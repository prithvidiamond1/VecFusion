void s1113_opt(int iterations, int LEN_1D, float* a, float* b)
{
    int mid = LEN_1D / 2;
    for (int nl = 0; nl < 2*iterations; nl++) {
        // Hoist a[mid] into scalar before any writes to a[i]
        float amid = a[mid];
        // Phase 1: i = 0 to mid-1, use hoisted scalar, fully vectorizable
        for (int i = 0; i < mid; i++) {
            a[i] = amid + b[i];
        }
        // Mid-point update
        a[mid] = amid + b[mid];
        float tmp = a[mid];
        // Phase 2: i = mid+1 to LEN_1D-1, use updated scalar, fully vectorizable
        for (int i = mid + 1; i < LEN_1D; i++) {
            a[i] = tmp + b[i];
        }
    }
}