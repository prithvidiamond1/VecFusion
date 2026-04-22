float s318_opt(int iterations, int LEN_1D, float* a, int inc) {
    int k, index;
    float max, chksum;
    
    for (int nl = 0; nl < iterations/2; nl++) {
        k = 0;
        index = 0;
        max = fabsf(a[0]);
        k += inc;
        
        // Split the loop: first handle the comparison values
        float max_val = max;
        int max_index = index;
        
        // Process all elements to find maximum absolute value
        for (int i = 1; i < LEN_1D; i++) {
            float current_abs = fabsf(a[k]);
            // Store comparison result
            int is_greater = (current_abs > max_val);
            // Update using arithmetic to avoid branching
            max_val = is_greater ? current_abs : max_val;
            k += inc;
        }
        
        // Second pass: find the first index where value equals max_val
        k = inc; // Reset to first element
        for (int i = 1; i < LEN_1D; i++) {
            float current_abs = fabsf(a[k]);
            if (current_abs == max_val) {
                max_index = i;
                break; // Found first occurrence
            }
            k += inc;
        }
        
        max = max_val;
        index = max_index;
        chksum = max + (float)index;
    }
    return max + index + 1;
}
