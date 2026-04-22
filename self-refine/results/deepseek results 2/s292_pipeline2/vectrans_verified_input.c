void s292(int iterations,int LEN_1D, float* a,float* b)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Precompute the two wrap-around indices for the first two elements
        float b_im1 = b[LEN_1D-1];
        float b_im2 = b[LEN_1D-2];
        
        // Handle i=0 separately (uses b[LEN_1D-1] and b[LEN_1D-2])
        a[0] = (b[0] + b_im1 + b_im2) * (float).333;
        
        // Handle i=1 separately (uses b[0] and b[LEN_1D-1])
        float b_i_minus_1 = b[0];
        a[1] = (b[1] + b_i_minus_1 + b_im1) * (float).333;
        
        // Main vectorizable loop for i=2..LEN_1D-1
        // No loop-carried dependency: uses b[i], b[i-1], b[i-2]
        for (int i = 2; i < LEN_1D; i++) {
            a[i] = (b[i] + b[i-1] + b[i-2]) * (float).333;
        }
    }
}