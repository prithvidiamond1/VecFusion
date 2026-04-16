void s451_opt(int iterations, int LEN_1D, float* __restrict__ a, float* __restrict__ b, float* __restrict__ c)
{
    for (int nl = 0; nl < iterations/5; nl++) {
        for (int i = 0; i < LEN_1D; i++) {
            a[i] = sinf(b[i]) + cosf(c[i]);
        }
    }
}
