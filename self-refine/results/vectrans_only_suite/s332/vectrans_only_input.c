
float s332_opt(int iterations, int LEN_1D, int t, float* a) {
    int index;
    float value;
    float chksum;
    
    for (int nl = 0; nl < iterations; nl++) {
        index = -2;
        value = -1.0f;
        
        // Vectorizable loop: find first element > t
        int found_index = LEN_1D;  // Use sentinel value
        
        // Single loop without early exit - fully vectorizable
        for (int i = 0; i < LEN_1D; i++) {
            // Branchless computation of minimum index where condition is true
            int cond = a[i] > t;
            // If condition is true and i < found_index, update found_index
            int candidate = cond ? i : LEN_1D;
            found_index = candidate < found_index ? candidate : found_index;
        }
        
        // Handle results after loop
        if (found_index < LEN_1D) {
            index = found_index;
            value = a[found_index];
        }
        
        chksum = value + (float)index;
    }
    return value;
}
