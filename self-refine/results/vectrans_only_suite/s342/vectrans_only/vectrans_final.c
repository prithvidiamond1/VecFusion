
void s342_opt(int iterations,int LEN_1D, float* a, float *b)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Temporary array to store original a values
        float orig_a[LEN_1D];
        
        // First loop: compute mask - fully vectorizable
        int mask[LEN_1D];
        for (int i = 0; i < LEN_1D; i++) {
            mask[i] = a[i] > (float)0.;
            orig_a[i] = a[i];
        }
        
        // Second loop: prefix sum to compute write indices - sequential
        int write_idx[LEN_1D];
        int count = 0;
        for (int i = 0; i < LEN_1D; i++) {
            write_idx[i] = count;
            count += mask[i];
        }
        
        // Third loop: compute new values without branching - vectorizable
        for (int i = 0; i < LEN_1D; i++) {
            // Use arithmetic masking instead of conditional branch
            float b_value = b[write_idx[i]];
            a[i] = mask[i] * b_value + (1 - mask[i]) * orig_a[i];
        }
    }
}
