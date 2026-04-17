float s318_opt(int iterations, int LEN_1D, float* a, int inc)
{
    int k, index;
    float max, chksum;
    for (int nl = 0; nl < iterations/2; nl++) {
        index = 0;
        max = fabsf(a[0]);

        // First pass: find max value (vectorizable max-reduction)
        for (int i = 1; i < LEN_1D; i++) {
            float val = fabsf(a[i * inc]);
            if (val > max) max = val;
        }

        // Second pass: find first index where value equals max
        // Use a flag array approach to avoid loop-carried dependency on index
        int found = 0;
        for (int i = 1; i < LEN_1D; i++) {
            float val = fabsf(a[i * inc]);
            int is_max = (val == max);
            int update = is_max & (found == 0);
            index = update ? i : index;
            found = found | update;
        }

        // Fix index: if max is a[0], index should remain 0
        index = (fabsf(a[0]) == max) ? 0 : index;

        chksum = max + (float) index;
    }
    return max + index + 1;
}
