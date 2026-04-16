void s341_opt(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Pass 1: Compute mask array (vectorizable - no dependencies)
        int mask[LEN_1D];
        for (int i = 0; i < LEN_1D; i++) {
            mask[i] = (b[i] > (float)0.) ? 1 : 0;
        }

        // Pass 2: Compute prefix sum to get output indices
        int prefix[LEN_1D];
        prefix[0] = mask[0] - 1;
        for (int i = 1; i < LEN_1D; i++) {
            prefix[i] = prefix[i-1] + mask[i];
        }

        // Pass 3: Write results using prefix as index directly
        for (int i = 0; i < LEN_1D; i++) {
            if (mask[i]) {
                a[prefix[i]] = b[i];
            }
        }
    }
}
