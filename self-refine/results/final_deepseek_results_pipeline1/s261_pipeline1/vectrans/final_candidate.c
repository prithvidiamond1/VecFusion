void s261_opt(int iterations,int LEN_1D, float* a,float* b,float* c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        // First loop: compute all c[i] values independently
        for (int i = 1; i < LEN_1D; ++i) {
            c[i] = c[i] * d[i];
        }
        
        // Second loop: compute all a[i] values using already updated c[i-1]
        for (int i = 1; i < LEN_1D; ++i) {
            a[i] = a[i] + b[i] + c[i-1];
        }
    }
}
