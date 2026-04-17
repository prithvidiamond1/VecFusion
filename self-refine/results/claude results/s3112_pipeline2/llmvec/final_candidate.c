#include <stdint.h>

typedef float float4 __attribute__((vector_size(16)));

float vectorized_s3112(int iterations, int LEN_1D, float* a, float* b)
{
    float sum;
    for (int nl = 0; nl < iterations; nl++) {
        sum = (float)0.0;

        // Pass 1 (vectorized): copy a[i] into b[i]
        int i = 0;
        for (; i <= LEN_1D - 4; i += 4) {
            float4 va;
            __builtin_memcpy(&va, &a[i], 16);
            __builtin_memcpy(&b[i], &va, 16);
        }
        for (; i < LEN_1D; i++) {
            b[i] = a[i];
        }

        int BLOCK = 1024;
        int nblocks = (LEN_1D + BLOCK - 1) / BLOCK;

        // Use VLAs for block sums
        float block_sum[nblocks];
        float block_carry[nblocks];

        // Pass 2a: compute local prefix sum within each block (sequential per block)
        for (int k = 0; k < nblocks; k++) {
            int start = k * BLOCK;
            int end = start + BLOCK < LEN_1D ? start + BLOCK : LEN_1D;
            float s = 0.0f;
            for (int j = start; j < end; j++) {
                s += b[j];
                b[j] = s;
            }
            block_sum[k] = s;
        }

        // Pass 2b (sequential): compute cumulative block carry offsets
        float carry = 0.0f;
        for (int k = 0; k < nblocks; k++) {
            block_carry[k] = carry;
            carry += block_sum[k];
        }

        // Pass 3 (vectorized): add block carry offset to each element
        for (int k = 0; k < nblocks; k++) {
            int start = k * BLOCK;
            int end = start + BLOCK < LEN_1D ? start + BLOCK : LEN_1D;
            float off = block_carry[k];

            // Broadcast offset into a vector
            float4 voff = {off, off, off, off};

            int j = start;
            for (; j <= end - 4; j += 4) {
                float4 vb;
                __builtin_memcpy(&vb, &b[j], 16);
                vb = vb + voff;
                __builtin_memcpy(&b[j], &vb, 16);
            }
            for (; j < end; j++) {
                b[j] += off;
            }
        }

        sum = b[LEN_1D - 1];
    }
    return sum;
}
