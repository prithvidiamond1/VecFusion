void s123_opt(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    int half_len = LEN_1D / 2;
    
    for (int nl = 0; nl < iterations; nl++) {
        int j = -1;
        
        // First pass: compute all d[i]*e[i] and b[i] + d[i]*e[i]
        for (int i = 0; i < half_len; i++) {
            float de = d[i] * e[i];
            float b_val = b[i] + de;
            
            // Store b_val in a temporary location (we'll move it later)
            // Use a as temporary storage for b_val at position i*2
            a[i*2] = b_val;
            
            // Also compute and store c[i] + d[i]*e[i] if needed
            if (c[i] > (float)0.) {
                float c_val = c[i] + de;
                // Store c_val in next temporary position
                a[i*2 + 1] = c_val;
            }
        }
        
        // Second pass: compact the values into correct positions
        j = -1;
        for (int i = 0; i < half_len; i++) {
            j++;
            // Write the b_val that was stored at i*2
            a[j] = a[i*2];
            
            if (c[i] > (float)0.) {
                j++;
                // Write the c_val that was stored at i*2+1
                a[j] = a[i*2 + 1];
            }
        }
    }
}
