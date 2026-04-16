void s2251_opt(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations; nl++) {
        // i=0: s=0.0 (initial value)
        a[0] = 0.0f * e[0];
        float s = b[0] + c[0];
        b[0] = a[0] + d[0];

        // For i>=1: a[i] = s_prev * e[i], where s_prev = b[i-1]_orig + c[i-1]
        // Note: b[i-1] is already updated at this point, so we need original b[i-1]
        // s carries the value b[i-1]_orig + c[i-1] from previous iteration
        for (int i = 1; i < LEN_1D; i++) {
            a[i] = s * e[i];
            s = b[i] + c[i];
            b[i] = a[i] + d[i];
        }
    }
}
