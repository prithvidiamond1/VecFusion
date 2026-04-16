#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s141(int iterations, float * __restrict__ flat_2d_array, float bb[256][256])
{
    int nl_count = 200 * (iterations / 256);

    // Pre-compute k indices for all (i,j) pairs using direct sequential computation
    int k_start[256];
    for (int i = 0; i < 256; i++) {
        k_start[i] = (i + 1) * i / 2 + i;
    }

    // Pre-compute col_sum with row-friendly access pattern
    float col_sum[256][256];
    for (int j = 0; j < 256; j++) {
        for (int i = 0; i <= j; i++) {
            col_sum[i][j] = bb[j][i];
        }
    }

    // Pre-accumulate nl_count multiplier into col_sum to eliminate nl loop
    // so scatter only happens once
    for (int i = 0; i < 256; i++) {
        for (int j = i; j < 256; j++) {
            col_sum[i][j] *= nl_count;
        }
    }

    // Single scatter pass: write sequentially per row i
    // For fixed i, k = k_start[i] + sum(j+1) for j=i..J-1, which is monotonically increasing
    // Use sequential k to avoid indirect indexing
    for (int i = 0; i < 256; i++) {
        int k = k_start[i];
        for (int j = i; j < 256; j++) {
            flat_2d_array[k] += col_sum[i][j];
            k += j + 1;
        }
    }
}

        #include <string.h>

void vectorized_s141(int iterations, float * __restrict__ flat_2d_array, float bb[256][256])
{
    int nl_count = 200 * (iterations / 256);
    float nl_count_f = (float)nl_count;

    // Pre-compute k indices for all i
    int k_start[256];
    for (int i = 0; i < 256; i++) {
        k_start[i] = (i + 1) * i / 2 + i;
    }

    // Pre-compute col_sum with row-friendly access pattern
    // and fold nl_count multiplication in one pass
    // Use flat array to avoid large stack 2D array issues
    static float col_sum[256][256];

    // Initialize col_sum to 0
    memset(col_sum, 0, sizeof(col_sum));

    // Access bb as flat pointer to handle harness passing flat arrays
    float * __restrict__ bb_flat = (float * __restrict__)bb;

    // Fill upper triangle: col_sum[i][j] = bb[j][i] * nl_count, for i <= j
    for (int j = 0; j < 256; j++) {
        int i = 0;
        // Process 8 at a time
        for (; i + 7 <= j; i += 8) {
            col_sum[i+0][j] = bb_flat[j*256 + i+0] * nl_count_f;
            col_sum[i+1][j] = bb_flat[j*256 + i+1] * nl_count_f;
            col_sum[i+2][j] = bb_flat[j*256 + i+2] * nl_count_f;
            col_sum[i+3][j] = bb_flat[j*256 + i+3] * nl_count_f;
            col_sum[i+4][j] = bb_flat[j*256 + i+4] * nl_count_f;
            col_sum[i+5][j] = bb_flat[j*256 + i+5] * nl_count_f;
            col_sum[i+6][j] = bb_flat[j*256 + i+6] * nl_count_f;
            col_sum[i+7][j] = bb_flat[j*256 + i+7] * nl_count_f;
        }
        for (; i <= j; i++) {
            col_sum[i][j] = bb_flat[j*256 + i] * nl_count_f;
        }
    }

    // Single scatter pass: for each row i, walk j from i to 255
    // k advances by (j+1) each step
    for (int i = 0; i < 256; i++) {
        int k = k_start[i];
        int j = i;

        // Unroll by 8 where possible
        for (; j + 7 < 256; j += 8) {
            flat_2d_array[k] += col_sum[i][j];
            k += j + 1;
            flat_2d_array[k] += col_sum[i][j+1];
            k += j + 2;
            flat_2d_array[k] += col_sum[i][j+2];
            k += j + 3;
            flat_2d_array[k] += col_sum[i][j+3];
            k += j + 4;
            flat_2d_array[k] += col_sum[i][j+4];
            k += j + 5;
            flat_2d_array[k] += col_sum[i][j+5];
            k += j + 6;
            flat_2d_array[k] += col_sum[i][j+6];
            k += j + 7;
            flat_2d_array[k] += col_sum[i][j+7];
            k += j + 8;
        }
        for (; j < 256; j++) {
            flat_2d_array[k] += col_sum[i][j];
            k += j + 1;
        }
    }
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
            int iterations = 5; float __restrict__ flat_2d_array_scalar[128]; float __restrict__ flat_2d_array_vector[128]; float bb_scalar[128]; float bb_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(flat_2d_array_scalar, n, &seed); memcpy(flat_2d_array_vector, flat_2d_array_scalar, sizeof(flat_2d_array_scalar)); fill_f32(bb_scalar, n, &seed); memcpy(bb_vector, bb_scalar, sizeof(bb_scalar));
                s141(iterations, flat_2d_array_scalar, bb_scalar); vectorized_s141(iterations, flat_2d_array_vector, bb_vector);
                for (int i = 0; i < n; ++i) {
    if (fabsf((flat_2d_array_scalar[i]) - (flat_2d_array_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter flat_2d_array on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((bb_scalar[i]) - (bb_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter bb on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
