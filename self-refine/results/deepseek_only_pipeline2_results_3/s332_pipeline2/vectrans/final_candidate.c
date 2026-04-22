float s332_opt(int iterations,int LEN_1D,int t, float* a)
{
    int index;
    float value;
    float chksum;
    
    for (int nl = 0; nl < iterations; nl++) {
        index = -2;
        value = -1.0f;
        
        // Loop distribution: separate the search for first match from value extraction
        int found_index = -2;
        
        // First loop: find the first index where a[i] > t (vectorizable search)
        for (int i = 0; i < LEN_1D; i++) {
            // Branchless comparison - creates mask of matches
            int match = a[i] > t;
            // Only update if we haven't found a match yet
            int update = match && (found_index == -2);
            // Use arithmetic to update index without branching
            found_index = update * i + (1 - update) * found_index;
        }
        
        // Second loop: extract the value if index was found
        if (found_index != -2) {
            index = found_index;
            value = a[found_index];
        }
        
        chksum = value + (float)index;
    }
    return value;
}
