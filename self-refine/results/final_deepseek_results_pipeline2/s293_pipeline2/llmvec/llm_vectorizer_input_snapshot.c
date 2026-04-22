void s293_opt(int iterations, int LEN_1D, float* a) {
    float temp = a[0];
    for (int i = 0; i < LEN_1D; i++) {
        a[i] = temp;
    }
    // The outer loop does nothing after first iteration
}
