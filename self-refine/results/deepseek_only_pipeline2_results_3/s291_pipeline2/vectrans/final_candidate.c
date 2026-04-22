void s291_opt(int iterations,int LEN_1D, float* a,float* b)
{
    for (int nl = 0; nl < 2*iterations; nl++) {
        // Handle first element separately (circular case)
        a[0] = (b[0] + b[LEN_1D-1]) * (float).5;
        
        // Main loop: now each iteration only depends on b[i] and b[i-1]
        // No loop-carried dependency, can be vectorized
        for (int i = 1; i < LEN_1D; i++) {
            a[i] = (b[i] + b[i-1]) * (float).5;
        }
    }
}
