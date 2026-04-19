float s316_opt(int iterations, int LEN_1D, float* a) {
    float x;
    for (int nl = 0; nl < iterations*5; nl++) {
        float min1 = a[0];
        float min2 = a[0];
        int i;
        
        // Process elements in pairs to reduce loop-carried dependency
        for (i = 1; i < LEN_1D-1; i += 2) {
            float val1 = a[i];
            float val2 = a[i+1];
            
            // Compare first element
            if (val1 < min1) {
                min1 = val1;
            }
            
            // Compare second element
            if (val2 < min2) {
                min2 = val2;
            }
        }
        
        // Handle remaining element if LEN_1D is odd
        if (i < LEN_1D) {
            float val = a[i];
            if (val < min1) {
                min1 = val;
            }
        }
        
        // Final reduction
        x = (min1 < min2) ? min1 : min2;
    }
    return x;
}
