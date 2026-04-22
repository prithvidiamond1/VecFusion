float s332(int iterations,int LEN_1D,int t, float* a)
{
    float value = 0.0f;
    float chksum = 0.0f;
    
    for (int nl = 0; nl < iterations; nl++) {
        int index = -2;
        float current_value = -1.0f;
        
        int i = 0;
        int found_index = LEN_1D;
        
        // Main vectorizable loop - process in chunks
        for (; i + 3 < LEN_1D; i += 4) {
            // Load 4 elements
            float a0 = a[i];
            float a1 = a[i+1];
            float a2 = a[i+2];
            float a3 = a[i+3];
            
            // Compute comparisons without branching
            int cond0 = a0 > t;
            int cond1 = a1 > t;
            int cond2 = a2 > t;
            int cond3 = a3 > t;
            
            // Calculate first match index in this chunk using arithmetic
            int first_in_chunk = LEN_1D;
            
            // Branchless calculation of first true index
            int mask0 = cond0;
            int mask1 = cond1 && !mask0;
            int mask2 = cond2 && !mask0 && !mask1;
            int mask3 = cond3 && !mask0 && !mask1 && !mask2;
            
            first_in_chunk = mask0 * i + mask1 * (i+1) + mask2 * (i+2) + mask3 * (i+3) + 
                           (!mask0 && !mask1 && !mask2 && !mask3) * LEN_1D;
            
            // Update global minimum index
            if (first_in_chunk < found_index) {
                found_index = first_in_chunk;
            }
        }
        
        // Remainder loop for cleanup
        for (; i < LEN_1D; i++) {
            if (a[i] > t) {
                if (i < found_index) {
                    found_index = i;
                }
                break;
            }
        }
        
        // Finalize results
        if (found_index < LEN_1D) {
            index = found_index;
            current_value = a[found_index];
        }
        
        value = current_value;
        chksum = value + (float)index;
    }
    return value;
}