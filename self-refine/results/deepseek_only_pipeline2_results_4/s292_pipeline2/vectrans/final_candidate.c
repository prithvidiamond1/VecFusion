void s292_opt(int iterations,int LEN_1D, float* a,float* b)
{
    int im1, im2;
    float prev2, prev1, current;
    
    for (int nl = 0; nl < iterations; nl++) {
        // Handle first two elements separately (special cases)
        if (LEN_1D > 0) {
            // i = 0: b[LEN_1D-1], b[LEN_1D-2]
            a[0] = (b[0] + b[LEN_1D-1] + b[LEN_1D-2]) * (float)0.333;
        }
        
        if (LEN_1D > 1) {
            // i = 1: b[1], b[LEN_1D-1], b[0]
            a[1] = (b[1] + b[0] + b[LEN_1D-1]) * (float)0.333;
        }
        
        // Main loop: i from 2 to LEN_1D-1
        // Now we have regular pattern: b[i], b[i-1], b[i-2]
        for (int i = 2; i < LEN_1D; i++) {
            a[i] = (b[i] + b[i-1] + b[i-2]) * (float)0.333;
        }
    }
}
