void s221(int iterations, int LEN_1D, float*a,float*b,float*c,float*d)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        // First compute all a[i] updates (independent)
        for (int i = 1; i < LEN_1D; i++) {
            a[i] += c[i] * d[i];
        }
        // Then compute b[i] with loop-carried dependency on b[i-1]
        for (int i = 1; i < LEN_1D; i++) {
            b[i] = b[i - 1] + a[i] + d[i];
        }
    }
}
