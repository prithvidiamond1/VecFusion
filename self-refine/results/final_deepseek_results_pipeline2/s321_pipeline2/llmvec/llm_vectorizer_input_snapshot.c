void s321_opt(int iterations, int LEN_1D, float* a, float* b) {
    for (int nl = 0; nl < iterations; nl++) {
        float prev = a[0];
        for (int i = 1; i < LEN_1D; i++) {
            float temp = a[i];
            a[i] = temp + prev * b[i];
            prev = a[i];
        }
    }
}
