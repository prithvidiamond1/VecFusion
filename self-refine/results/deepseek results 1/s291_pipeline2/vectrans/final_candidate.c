void s291_opt(int iterations,int LEN_1D, float* a,float* b)
{
    int im1;
    for (int nl = 0; nl < 2*iterations; nl++) {
        im1 = LEN_1D-1;
        // Handle first iteration separately to break dependency
        a[0] = (b[0] + b[im1]) * (float).5;
        // Main loop: im1 is now i-1, which is already computed
        for (int i = 1; i < LEN_1D; i++) {
            a[i] = (b[i] + b[i-1]) * (float).5;
        }
    }
}
