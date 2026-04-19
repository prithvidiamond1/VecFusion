float s318_opt(int iterations,int LEN_1D, float* a, int inc)
{
    int k, index;
    float max, chksum;
    for (int nl = 0; nl < iterations/2; nl++) {
        k = 0;
        index = 0;
        max = fabsf(a[0]);
        k += inc;
        
        // Precompute all absolute values into a temporary array
        // This allows vectorization of fabsf calls
        float* abs_vals = (float*)__builtin_alloca(LEN_1D * sizeof(float));
        int current_k = k;
        for (int i = 1; i < LEN_1D; i++) {
            abs_vals[i] = fabsf(a[current_k]);
            current_k += inc;
        }
        
        // Separate reduction into two independent loops
        // First: find maximum value (simple reduction pattern)
        float max_val = max;
        for (int i = 1; i < LEN_1D; i++) {
            float abs_val = abs_vals[i];
            if (abs_val > max_val) {
                max_val = abs_val;
            }
        }
        
        // Second: find first index where value equals maximum
        // This avoids the complex argmax reduction pattern
        int max_idx = 0;
        for (int i = 1; i < LEN_1D; i++) {
            if (abs_vals[i] == max_val) {
                max_idx = i;
                break;  // Only need first occurrence
            }
        }
        
        // Update original variables
        index = max_idx;
        max = max_val;
        chksum = max + (float) index;
    }
    return max + index + 1;
}
