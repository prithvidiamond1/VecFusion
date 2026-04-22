float s332_opt(int iterations,int LEN_1D,int t, float* a)
{
    int index;
    float value;
    float chksum;
    
    for (int nl = 0; nl < iterations; nl++) {
        index = -2;
        value = -1.0f;
        
        // Vectorizable min-reduction pattern
        int first_index = LEN_1D;
        for (int i = 0; i < LEN_1D; i++) {
            int candidate = (a[i] > t) ? i : LEN_1D;
            first_index = (candidate < first_index) ? candidate : first_index;
        }
        
        // Sequential extraction (minimal work)
        if (first_index < LEN_1D) {
            index = first_index;
            value = a[first_index];
        }
        
        chksum = value + (float)index;
    }
    return value;
}
