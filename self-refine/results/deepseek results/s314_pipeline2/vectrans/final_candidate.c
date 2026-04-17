float s314_opt(int iterations, int LEN_1D, float* a)
{
    float x;
    float max_val;
    
    for (int nl = 0; nl < iterations*5; nl++) {
        max_val = a[0];
        
        // Main vectorizable reduction loop
        for (int i = 0; i < LEN_1D; i++) {
            float current = a[i];
            if (current > max_val) {
                max_val = current;
            }
        }
        
        x = max_val;
    }
    return x;
}
