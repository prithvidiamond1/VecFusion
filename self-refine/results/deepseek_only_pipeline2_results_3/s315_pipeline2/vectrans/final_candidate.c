float s315_opt(int iterations, int LEN_1D, float* a) {
    // Initialization loop - already vectorizable
    for (int i = 0; i < LEN_1D; i++) {
        a[i] = (i * 7) % LEN_1D;
    }

    float x;
    int index;
    
    // Main loop with preserved semantics
    for (int nl = 0; nl < iterations; nl++) {
        // Initialize reduction variables
        x = a[0];
        index = 0;
        
        // Split the reduction: first find max value only
        float max_val = a[0];
        for (int i = 0; i < LEN_1D; ++i) {
            float current = a[i];
            if (current > max_val) {
                max_val = current;
            }
        }
        
        // Then find the first index where value equals max_val
        // This loop has no loop-carried dependency on index
        for (int i = 0; i < LEN_1D; ++i) {
            if (a[i] == max_val) {
                index = i;
                break;  // Get first occurrence
            }
        }
        
        x = max_val;
        // chksum computation (kept for semantic equivalence)
        float chksum = x + (float)index;
    }
    
    return index + x + 1;
}
