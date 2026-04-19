float s315(int iterations, int LEN_1D, float* a) {
    // Initialize array (vectorizable)
    for (int i = 0; i < LEN_1D; i++) {
        a[i] = (i * 7) % LEN_1D;
    }

    float x;
    int index;
    float chksum = 0.0f;

    // Outer loop over iterations
    for (int nl = 0; nl < iterations; nl++) {
        x = a[0];
        index = 0;
        
        // Loop splitting: separate max value reduction from index tracking
        // First loop: find max value (pure reduction, vectorizable)
        float max_val = a[0];
        for (int i = 1; i < LEN_1D; ++i) {
            float val = a[i];
            max_val = val > max_val ? val : max_val;
        }
        
        // Second loop: find last index of max value (vectorizable comparison)
        // Use branchless logic to track last occurrence
        int max_idx = 0;
        for (int i = 0; i < LEN_1D; ++i) {
            // Create mask: 1 if current element equals max_val, 0 otherwise
            int is_max = a[i] == max_val;
            // Update index to current i when is_max is true, keep old value otherwise
            // This captures the LAST occurrence since we overwrite with later indices
            max_idx = is_max ? i : max_idx;
        }
        
        x = max_val;
        index = max_idx;
        
        // Compute chksum to maintain exact memory access pattern
        chksum = x + (float)index;
        (void)chksum;
    }
    
    return index + x + 1;
}