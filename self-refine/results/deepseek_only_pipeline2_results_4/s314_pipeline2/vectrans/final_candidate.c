float s314_opt(int iterations, int LEN_1D, float* a)
{
    float x;
    for (int nl = 0; nl < iterations*5; nl++) {
        float max_val = a[0];
        
        // Main vectorizable reduction loop
        int i;
        for (i = 0; i < LEN_1D; i++) {
            float v = a[i];
            if (v > max_val) {
                max_val = v;
            }
        }
        
        x = max_val;
    }
    return x;
}
