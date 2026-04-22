float s318_opt(int iterations,int LEN_1D, float* a, int inc) {
    int k, index;
    float max, chksum;
    for (int nl = 0; nl < iterations/2; nl++) {
        k = 0;
        index = 0;
        max = fabsf(a[0]);
        k += inc;
        
        // First loop: compute all absolute values and find max (reduction)
        float local_max = max;
        for (int i = 1; i < LEN_1D; i++) {
            float abs_val = fabsf(a[k]);
            if (abs_val > local_max) {
                local_max = abs_val;
            }
            k += inc;
        }
        
        // Reset k for second loop
        k = 0;
        
        // Second loop: find first index where value equals max
        // Vectorizable version using masked min reduction
        int min_index = LEN_1D; // Initialize with invalid large index
        for (int i = 0; i < LEN_1D; i++) {
            float abs_val = fabsf(a[k]);
            int match = (abs_val == local_max);
            // If match is true and i < current min_index, update min_index
            min_index = match && (i < min_index) ? i : min_index;
            k += inc;
        }
        index = (min_index < LEN_1D) ? min_index : 0;
        
        max = local_max;
        chksum = max + (float) index;
    }
    return max + index + 1;
}
