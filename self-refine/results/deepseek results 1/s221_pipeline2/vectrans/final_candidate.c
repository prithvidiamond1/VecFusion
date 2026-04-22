void s221_opt(int iterations, int LEN_1D, float*a,float*b,float*c,float*d)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        // Split the loop: first compute independent a[i] updates
        for (int i = 1; i < LEN_1D; i++) {
            a[i] += c[i] * d[i];
        }
        // Then compute the b[i] recurrence (serial dependency preserved)
        for (int i = 1; i < LEN_1D; i++) {
            b[i] = b[i - 1] + a[i] + d[i];
        }
    }
}
