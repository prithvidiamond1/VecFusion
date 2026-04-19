float s318_opt(int iterations, int LEN_1D, float* a, int inc)
{
    int k, index;
    float max, chksum;
    
    for (int nl = 0; nl < iterations/2; nl++) {
        k = 0;
        index = 0;
        max = fabsf(a[0]);
        k += inc;
        
        // Split loop: first find max value (reduction)
        float local_max = max;
        int local_k = k;
        
        // Vectorizable reduction loop for max value only
        for (int i = 1; i < LEN_1D; i++) {
            float abs_val = fabsf(a[local_k]);
            local_max = (abs_val > local_max) ? abs_val : local_max;
            local_k += inc;
        }
        
        // Second pass: find index of max value
        // This can run after max is known
        k = inc; // Reset k for index search
        for (int i = 1; i < LEN_1D; i++) {
            float abs_val = fabsf(a[k]);
            // Use comparison with pre-computed max
            if (abs_val == local_max) {
                index = i;
                break; // Found first occurrence
            }
            k += inc;
        }
        
        max = local_max;
        chksum = max + (float)index;
    }
    return max + index + 1;
}
