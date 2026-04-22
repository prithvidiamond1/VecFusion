
void s261_opt(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Loop distribution: separate independent updates
        // First update all c[i] (independent)
        for (int i = 1; i < LEN_1D; ++i) {
            c[i] = c[i] * d[i];
        }
        // Then update all a[i] using previously updated c[i-1]
        for (int i = 1; i < LEN_1D; ++i) {
            a[i] = (a[i] + b[i]) + c[i-1];
        }
    }
}
