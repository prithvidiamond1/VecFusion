void s293_opt(int iterations,int LEN_1D, float* a) {
    for (int nl = 0; nl < 4*iterations; nl++) {
        float val = a[0];
        for (int i = 0; i < LEN_1D; i++) {
            a[i] = val;
        }
    }
}
