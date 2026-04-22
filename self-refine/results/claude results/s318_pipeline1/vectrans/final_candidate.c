float s318_opt(int iterations, int LEN_1D, float* a, int inc)
{
    int k, index;
    float max, chksum;
    for (int nl = 0; nl < iterations/2; nl++) {
        // First pass: find the maximum value (vectorizable reduction)
        float cur_max = fabsf(a[0]);
        k = inc;
        for (int i = 1; i < LEN_1D; i++) {
            float abs_val = fabsf(a[k]);
            cur_max = abs_val > cur_max ? abs_val : cur_max;
            k += inc;
        }
        max = cur_max;

        // Second pass: find the first index i>=1 where fabsf(a[i*inc]) == cur_max
        // and cur_max > fabsf(a[0]) (i.e., it was strictly greater at some point)
        // Use arithmetic masking: found_index holds first occurrence
        int found_index = 0;
        if (cur_max > fabsf(a[0])) {
            int first_found = LEN_1D; // sentinel: not found yet
            k = inc;
            for (int i = 1; i < LEN_1D; i++) {
                float abs_val = fabsf(a[k]);
                // Update first_found only if not yet found and abs_val == cur_max
                int is_match = (abs_val == cur_max);
                int not_yet_found = (first_found == LEN_1D);
                first_found = (is_match & not_yet_found) ? i : first_found;
                k += inc;
            }
            found_index = (first_found == LEN_1D) ? 0 : first_found;
        }
        index = found_index;

        chksum = max + (float) index;
    }
    return max + index + 1;
}
