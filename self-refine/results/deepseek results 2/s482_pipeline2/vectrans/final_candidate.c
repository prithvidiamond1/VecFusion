void s482_opt(int iterations, int LEN_1D, float* a, float *b, float *c)
{
    for (int nl = 0; nl < iterations; nl++) {
        int break_idx = LEN_1D;
        
        // First loop: perform updates and record break condition
        for (int i = 0; i < LEN_1D; i++) {
            a[i] += b[i] * c[i];
            if (c[i] > b[i]) {
                break_idx = i;
                break;
            }
        }
        
        // Second loop: continue from where we left off
        for (int i = break_idx + 1; i < LEN_1D; i++) {
            // No operation needed - elements after break remain unchanged
        }
    }
}
