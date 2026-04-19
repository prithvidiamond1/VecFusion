void s222_opt(int iterations, int LEN_1D, float*a,float*b,float*c, float* e) {
    for (int nl = 0; nl < iterations/2; nl++) {
        // Loop distribution: separate the e recurrence (loop-carried dependency)
        for (int i = 1; i < LEN_1D; i++) {
            e[i] = e[i - 1] * e[i - 1];
        }
        // Independent a updates (no net effect, but preserve memory access pattern)
        for (int i = 1; i < LEN_1D; i++) {
            float temp = b[i] * c[i];
            a[i] += temp;
            a[i] -= temp;
        }
    }
}
