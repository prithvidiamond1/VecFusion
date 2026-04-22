void s342(int iterations,int LEN_1D, float* a, float *b)
{
    for (int nl = 0; nl < iterations; nl++) {
        // First loop: compute mask and store original a values
        int mask[LEN_1D];
        float orig_a[LEN_1D];
        
        for (int i = 0; i < LEN_1D; i++) {
            mask[i] = (a[i] > (float)0.);
            orig_a[i] = a[i];
        }
        
        // Second loop: compute prefix sum (j values) using mask
        int j = -1;
        int j_vals[LEN_1D];
        
        for (int i = 0; i < LEN_1D; i++) {
            j_vals[i] = mask[i] ? (++j) : -1;
        }
        
        // Third loop: assign b values using computed indices
        for (int i = 0; i < LEN_1D; i++) {
            int idx = j_vals[i];
            a[i] = (idx != -1) ? b[idx] : orig_a[i];
        }
    }
}