void s342(int iterations,int LEN_1D, float* a, float *b)
{
    for (int nl = 0; nl < iterations; nl++) {
        // First pass: compute mask (vectorizable)
        char mask[LEN_1D];
        for (int i = 0; i < LEN_1D; i++) {
            mask[i] = (a[i] > (float)0.);
        }
        
        // Second pass: compute prefix sum of mask (j values)
        int indices[LEN_1D];
        int j = -1;
        for (int i = 0; i < LEN_1D; i++) {
            j += mask[i];
            indices[i] = j;
        }
        
        // Third pass: apply assignments using precomputed indices
        // Remove conditional branch to enable vectorization
        for (int i = 0; i < LEN_1D; i++) {
            // Use mask as selector: if mask[i] is 1, use b[indices[i]], else keep a[i]
            int idx = indices[i];
            float new_val = b[idx];
            float old_val = a[i];
            // Branchless selection using mask
            a[i] = mask[i] ? new_val : old_val;
        }
    }
}