float s318_opt(int iterations,int LEN_1D, float* a, int inc) {
    int k, index;
    float max, chksum;
    
    for (int nl = 0; nl < iterations/2; nl++) {
        k = 0;
        index = 0;
        max = fabsf(a[0]);
        
        // First pass: find maximum absolute value (reduction)
        float max_val = max;
        for (int i = 1; i < LEN_1D; i++) {
            k += inc;
            float abs_val = fabsf(a[k]);
            if (abs_val > max_val) {
                max_val = abs_val;
            }
        }
        
        // Second pass: find first index where value equals max_val
        k = 0;
        int found_idx = 0;
        for (int i = 0; i < LEN_1D; i++) {
            float abs_val = fabsf(a[k]);
            if (abs_val == max_val && found_idx == 0) {
                found_idx = i;
            }
            k += inc;
        }
        
        max = max_val;
        index = found_idx;
        chksum = max + (float) index;
    }
    return max + index + 1;
}
