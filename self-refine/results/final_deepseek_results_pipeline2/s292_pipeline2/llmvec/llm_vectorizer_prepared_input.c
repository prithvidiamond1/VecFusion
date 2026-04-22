void s292(int iterations,int LEN_1D, float* a,float* b)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Handle first two elements with wrap-around separately
        a[0] = (b[0] + b[LEN_1D-1] + b[LEN_1D-2]) * (float).333;
        a[1] = (b[1] + b[0] + b[LEN_1D-1]) * (float).333;
        
        // Main vectorizable loop for i=2 to LEN_1D-1
        for (int i = 2; i < LEN_1D; i++) {
            a[i] = (b[i] + b[i-1] + b[i-2]) * (float).333;
        }
    }
}
