float s3112_opt(int iterations, int LEN_1D, float* a, float* b)
{
    float sum;
    for (int nl = 0; nl < iterations; nl++) {
        sum = (float)0.0;

        // Pass 1 (vectorizable): copy a[i] into b[i]
        for (int i = 0; i < LEN_1D; i++) {
            b[i] = a[i];
        }

        int BLOCK = 1024;
        int nblocks = (LEN_1D + BLOCK - 1) / BLOCK;
        float block_sum[nblocks];

        // Pass 2a: compute local prefix sum within each block (sequential per block, but short)
        for (int k = 0; k < nblocks; k++) {
            int start = k * BLOCK;
            int end = start + BLOCK < LEN_1D ? start + BLOCK : LEN_1D;
            float s = 0.0f;
            for (int i = start; i < end; i++) {
                s += b[i];
                b[i] = s;
            }
            block_sum[k] = s;
        }

        // Pass 2b (sequential): compute cumulative block carry offsets
        float carry = 0.0f;
        float block_carry[nblocks];
        for (int k = 0; k < nblocks; k++) {
            block_carry[k] = carry;
            carry += block_sum[k];
        }

        // Pass 3 (vectorizable): add block carry offset to each element
        for (int k = 0; k < nblocks; k++) {
            int start = k * BLOCK;
            int end = start + BLOCK < LEN_1D ? start + BLOCK : LEN_1D;
            float off = block_carry[k];
            for (int i = start; i < end; i++) {
                b[i] += off;
            }
        }

        sum = b[LEN_1D - 1];
    }
    return sum;
}
