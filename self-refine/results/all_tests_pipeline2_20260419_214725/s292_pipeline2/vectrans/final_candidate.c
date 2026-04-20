void s292_opt(int iterations,int LEN_1D, float* a,float* b)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Handle first two iterations separately (special wrap-around cases)
        // i = 0
        a[0] = (b[0] + b[LEN_1D-1] + b[LEN_1D-2]) * (float).333;
        
        // i = 1
        a[1] = (b[1] + b[0] + b[LEN_1D-1]) * (float).333;
        
        // Main vectorizable loop for i >= 2
        // No loop-carried dependencies here since we use direct indexing
        for (int i = 2; i < LEN_1D; i++) {
            a[i] = (b[i] + b[i-1] + b[i-2]) * (float).333;
        }
    }
}
