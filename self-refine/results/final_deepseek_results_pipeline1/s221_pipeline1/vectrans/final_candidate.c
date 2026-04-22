void s221_opt(int iterations, int LEN_1D, float*a,float*b,float*c,float*d)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        // First loop: compute a[i] (vectorizable - no loop-carried dependency)
        for (int i = 1; i < LEN_1D; i++) {
            a[i] += c[i] * d[i];
        }
        // Second loop: compute b[i] with recurrence (cannot be vectorized)
        for (int i = 1; i < LEN_1D; i++) {
            b[i] = b[i - 1] + a[i] + d[i];
        }
    }
}
