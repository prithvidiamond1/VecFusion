float s316_opt(int iterations, int LEN_1D, float* a)
{
    float x;
    float min_val;
    
    for (int nl = 0; nl < iterations*5; nl++) {
        min_val = a[0];
        
        // Process first element separately
        x = min_val;
        
        // Main loop without branching - enables vectorization
        for (int i = 1; i < LEN_1D; ++i) {
            float current = a[i];
            // Use conditional move pattern (compiler can optimize to min instruction)
            if (current < min_val) {
                min_val = current;
            }
        }
        
        x = min_val;
    }
    return x;
}
