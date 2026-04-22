float s332_opt(int iterations,int LEN_1D,int t, float* a)
{
    int index;
    float value;
    float chksum;
    
    for (int nl = 0; nl < iterations; nl++) {
        index = -2;
        value = -1.0f;
        
        // Process in fixed-size chunks without early exit
        int found_index = -1;
        const int CHUNK_SIZE = 8; // SIMD-friendly fixed size
        
        // First loop: process full chunks without breaking
        int i;
        for (i = 0; i <= LEN_1D - CHUNK_SIZE; i += CHUNK_SIZE) {
            // Process chunk - all iterations independent
            int chunk_found = -1;
            float chunk_value = -1.0f;
            
            // Unrolled chunk processing (no early exit within chunk)
            for (int j = 0; j < CHUNK_SIZE; j++) {
                int current_idx = i + j;
                float current_val = a[current_idx];
                int is_greater = (current_val > t);
                
                // Track first match in chunk using conditional update
                if (is_greater && chunk_found == -1) {
                    chunk_found = current_idx;
                    chunk_value = current_val;
                }
            }
            
            // After processing chunk, check if found
            if (chunk_found != -1) {
                found_index = chunk_found;
                value = chunk_value;
                break;
            }
        }
        
        // Second loop: process remaining elements
        if (found_index == -1) {
            int start_remain = i;
            for (int j = start_remain; j < LEN_1D; j++) {
                if (a[j] > t) {
                    found_index = j;
                    value = a[j];
                    break;
                }
            }
        }
        
        // Set final results
        if (found_index != -1) {
            index = found_index;
            // value already set above
        }
        
        chksum = value + (float)index;
    }
    
    return value;
}
