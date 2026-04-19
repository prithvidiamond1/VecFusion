void s291(int iterations,int LEN_1D, float* a,float* b)
{
    int im1;
    float temp;
    
    for (int nl = 0; nl < 2*iterations; nl++) {
        im1 = LEN_1D-1;
        
        // Handle first iteration separately to break dependency
        a[0] = (b[0] + b[im1]) * (float).5;
        
        // Main loop - reordered to remove loop-carried dependency
        for (int i = 1; i < LEN_1D; i++) {
            // Store current im1 value before updating
            temp = b[i-1];
            // Update a[i] using stored value
            a[i] = (b[i] + temp) * (float).5;
        }
    }
}