#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s141(int iterations, float * flat_2d_array, float bb[256][256])
{
    int nl_iter = 200 * (iterations / 256);

    // Precompute all k indices for each (i,j) pair
    int k_indices[256][256];
    for (int i = 0; i < 256; i++) {
        int k = (i+1) * i / 2 + i;
        for (int j = i; j < 256; j++) {
            k_indices[i][j] = k;
            k += j + 1;
        }
    }

    // Loop distribution: separate accumulation from index calculation
    for (int nl = 0; nl < nl_iter; nl++) {
        // First loop: accumulate contributions in a vector-friendly way
        for (int i = 0; i < 256; i++) {
            // Temporary array for contiguous accumulation
            float temp_accum[256];

            // Vectorizable accumulation of bb values - use memcpy for column access
            // Load bb column i into temp_accum starting at position i
            for (int j = i; j < 256; j++) {
                temp_accum[j] = bb[j][i];
            }

            // Scatter accumulated values to flat_2d_array
            // This loop has indirect addressing but is independent of the first loop
            for (int j = i; j < 256; j++) {
                int k = k_indices[i][j];
                flat_2d_array[k] += temp_accum[j];
            }
        }
    }
}

        #include <string.h>

void vectorized_s141(int iterations, float * flat_2d_array, float bb[256][256])
{
    int nl_iter = 200 * (iterations / 256);

    // Precompute all k indices for each (i,j) pair
    int k_indices[256][256];
    for (int i = 0; i < 256; i++) {
        int k = (i+1) * i / 2 + i;
        for (int j = i; j < 256; j++) {
            k_indices[i][j] = k;
            k += j + 1;
        }
    }

    for (int nl = 0; nl < nl_iter; nl++) {
        for (int i = 0; i < 256; i++) {
            float temp_accum[256];

            // Vectorizable: load bb[j][i] for j = i..255
            // This is a column access, stride = 256 floats
            // We unroll manually to help the compiler vectorize
            int j = i;
            int len = 256 - i;

            // Unrolled by 8 for vectorization hint
            int j8_end = i + (len & ~7);
            for (; j < j8_end; j += 8) {
                temp_accum[j+0] = bb[j+0][i];
                temp_accum[j+1] = bb[j+1][i];
                temp_accum[j+2] = bb[j+2][i];
                temp_accum[j+3] = bb[j+3][i];
                temp_accum[j+4] = bb[j+4][i];
                temp_accum[j+5] = bb[j+5][i];
                temp_accum[j+6] = bb[j+6][i];
                temp_accum[j+7] = bb[j+7][i];
            }
            for (; j < 256; j++) {
                temp_accum[j] = bb[j][i];
            }

            // Scatter: indirect addressing, hard to vectorize, keep scalar
            for (j = i; j < 256; j++) {
                int k = k_indices[i][j];
                flat_2d_array[k] += temp_accum[j];
            }
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
            const int arr_len = 128;
            uint32_t seed = 7u;
            int iterations = 5; float flat_2d_array_scalar[128]; float flat_2d_array_vector[128]; float bb_scalar[128]; float bb_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(flat_2d_array_scalar, arr_len, &seed); memcpy(flat_2d_array_vector, flat_2d_array_scalar, sizeof(flat_2d_array_scalar)); fill_f32(bb_scalar, arr_len, &seed); memcpy(bb_vector, bb_scalar, sizeof(bb_scalar));
                s141(iterations, flat_2d_array_scalar, bb_scalar); vectorized_s141(iterations, flat_2d_array_vector, bb_vector);
                for (int i = 0; i < arr_len; ++i) {
    if (fabsf((flat_2d_array_scalar[i]) - (flat_2d_array_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter flat_2d_array on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((bb_scalar[i]) - (bb_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter bb on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
