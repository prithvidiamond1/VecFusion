float s315(int iterations, int LEN_1D, float* a)
{
    for (int i = 0; i < LEN_1D; i++)
        a[i] = (i * 7) % LEN_1D;

    float x, chksum;
    int index;
    for (int nl = 0; nl < iterations; nl++) {
        /* First pass: find the maximum value using a pure float reduction (vectorizable) */
        float max_val = a[0];
        for (int i = 1; i < LEN_1D; i++) {
            max_val = a[i] > max_val ? a[i] : max_val;
        }

        /* Second pass: find minimum index where a[i] == max_val
           Use negated-index max reduction: max of (-i) where a[i]==max_val
           Equivalent to finding the first (minimum) occurrence index.
           neg_idx starts at 0 (corresponds to index 0).
           For each i, if a[i]==max_val and -i > neg_idx, update neg_idx = -i.
           This is a standard max-reduction, vectorizable. */
        int neg_idx = 0; /* -index, starts at -(0) = 0 */
        for (int i = 0; i < LEN_1D; i++) {
            int c = (a[i] == max_val);
            int candidate = -i;
            neg_idx = (c & (candidate > neg_idx)) ? candidate : neg_idx;
        }

        x = max_val;
        index = -neg_idx;
        chksum = x + (float)index;
    }
    return index + x + 1;
}