void s261_opt(int iterations,int LEN_1D, float* a,float* b,float* c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Reorder instructions to break the loop-carried dependency on c[i]
        // First compute all new c[i] values, then compute a[i] using updated c[i-1]
        for (int i = 1; i < LEN_1D; ++i) {
            c[i] = c[i] * d[i];
        }
        for (int i = 1; i < LEN_1D; ++i) {
            a[i] = (a[i] + b[i]) + c[i-1];
        }
    }
}
