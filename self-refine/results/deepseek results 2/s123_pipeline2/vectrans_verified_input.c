void s123(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    int half_len = LEN_1D/2;
    
    for (int nl = 0; nl < iterations; nl++) {
        // Precompute d[i]*e[i] and condition mask
        float de[half_len];
        int mask[half_len];
        
        // Vectorizable loop: independent computations
        for (int i = 0; i < half_len; i++) {
            de[i] = d[i] * e[i];
            mask[i] = (c[i] > (float)0.);
        }
        
        // Compute write positions using prefix sum
        int write_pos[half_len];
        int current_j = -1;
        for (int i = 0; i < half_len; i++) {
            current_j++;
            write_pos[i] = current_j;
            if (mask[i]) {
                current_j++;
            }
        }
        
        // First write: always write b[i] + de[i]
        for (int i = 0; i < half_len; i++) {
            a[write_pos[i]] = b[i] + de[i];
        }
        
        // Second write: conditional write c[i] + de[i]
        for (int i = 0; i < half_len; i++) {
            if (mask[i]) {
                a[write_pos[i] + 1] = c[i] + de[i];
            }
        }
    }
}