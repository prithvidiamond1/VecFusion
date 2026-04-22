
void s161_opt(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e) {
    for (int nl = 0; nl < iterations/2; nl++) {
        // First: compute c[i+1] updates where b[i] < 0 (using old a[i])
        for (int i = 0; i < LEN_1D-1; ++i) {
            if (b[i] < (float)0.) {
                c[i+1] = a[i] + d[i] * d[i];
            }
        }
        
        // Second: compute a[i] updates where b[i] >= 0 (using potentially updated c[i])
        for (int i = 0; i < LEN_1D-1; ++i) {
            if (b[i] >= (float)0.) {
                a[i] = c[i] + d[i] * e[i];
            }
        }
    }
}
