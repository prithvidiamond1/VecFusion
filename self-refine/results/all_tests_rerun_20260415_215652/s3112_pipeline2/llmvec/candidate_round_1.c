#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s3112(int iterations, int LEN_1D, float* a, float* b)
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

        static uint32_t next_u32(uint32_t *state) {
            *state = (*state * 1664525u) + 1013904223u;
            return *state;
        }

        static void fill_i32(int *buf, int n, uint32_t *state) {
            for (int i = 0; i < n; ++i) {
                buf[i] = (int)(next_u32(state) % 2001u) - 1000;
            }
        }

        static void fill_f32(float *buf, int n, uint32_t *state) {
            for (int i = 0; i < n; ++i) {
                buf[i] = ((float)(next_u32(state) % 2001u) - 1000.0f) / 17.0f;
            }
        }

        static void fill_f64(double *buf, int n, uint32_t *state) {
            for (int i = 0; i < n; ++i) {
                buf[i] = ((double)(next_u32(state) % 2001u) - 1000.0) / 17.0;
            }
        }

        int main(void) {
            const int n = 128;
            uint32_t seed = 7u;
            int iterations = 5; int LEN_1D = n; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float ret_scalar; float ret_vector;

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, n, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar));
                ret_scalar = s3112(iterations, LEN_1D, a_scalar, b_scalar); ret_vector = vectorized_s3112(iterations, LEN_1D, a_vector, b_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
    fprintf(stderr, "Return mismatch on trial %d\n", trial);
    return 2;
}
                for (int i = 0; i < n; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((b_scalar[i]) - (b_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter b on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
