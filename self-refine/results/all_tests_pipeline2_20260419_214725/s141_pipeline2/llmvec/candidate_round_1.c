#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s141(int iterations, float * flat_2d_array, float bb[256][256])
{
    int nl_max = 200 * (iterations / 256);

    // Precompute k indices once
    int k_indices[256][256];
    for (int i = 0; i < 256; i++) {
        int k = i * (i + 3) / 2;
        for (int j = i; j < 256; j++) {
            k_indices[i][j] = k;
            k += j + 1;
        }
    }

    // Process outer loop
    for (int nl = 0; nl < nl_max; nl++) {
        // Loop distribution with temporary accumulation
        for (int i = 0; i < 256; i++) {
            float temp_accum[256 - i];

            // First: gather all bb values (fully vectorizable)
            for (int j = i; j < 256; j++) {
                temp_accum[j - i] = bb[j][i];
            }

            // Second: accumulate using precomputed indices
            // Use local pointer for better aliasing analysis
            float *local_flat = flat_2d_array;
            const int *local_k_idx = &k_indices[i][i];
            for (int j = 0; j < 256 - i; j++) {
                local_flat[local_k_idx[j]] += temp_accum[j];
            }
        }
    }
}

        #include <stddef.h>

void vectorized_s141(int iterations, float * flat_2d_array, float bb[256][256])
{
    int nl_max = 200 * (iterations / 256);

    // Precompute k indices once
    int k_indices[256][256];
    for (int i = 0; i < 256; i++) {
        int k = i * (i + 3) / 2;
        for (int j = i; j < 256; j++) {
            k_indices[i][j] = k;
            k += j + 1;
        }
    }

    // Process outer loop
    for (int nl = 0; nl < nl_max; nl++) {
        // Loop distribution with temporary accumulation
        for (int i = 0; i < 256; i++) {
            float temp_accum[256 - i];

            // First: gather all bb values (fully vectorizable)
            // Canonical counted loop with explicit bounds
            int j_start = i;
            int j_end = 256;
            int j_count = j_end - j_start;

            // Vector-friendly loop with cleanup
            int j;
            for (j = j_start; j + 3 < j_end; j += 4) {
                temp_accum[j - i] = bb[j][i];
                temp_accum[j - i + 1] = bb[j + 1][i];
                temp_accum[j - i + 2] = bb[j + 2][i];
                temp_accum[j - i + 3] = bb[j + 3][i];
            }
            // Scalar cleanup
            for (; j < j_end; j++) {
                temp_accum[j - i] = bb[j][i];
            }

            // Second: accumulate using precomputed indices
            // Use local pointer for better aliasing analysis
            float *local_flat = flat_2d_array;
            const int *local_k_idx = &k_indices[i][i];

            // Canonical counted loop with explicit bounds
            int accum_count = 256 - i;

            // Vector-friendly loop with cleanup
            int idx;
            for (idx = 0; idx + 3 < accum_count; idx += 4) {
                int k0 = local_k_idx[idx];
                int k1 = local_k_idx[idx + 1];
                int k2 = local_k_idx[idx + 2];
                int k3 = local_k_idx[idx + 3];

                local_flat[k0] += temp_accum[idx];
                local_flat[k1] += temp_accum[idx + 1];
                local_flat[k2] += temp_accum[idx + 2];
                local_flat[k3] += temp_accum[idx + 3];
            }
            // Scalar cleanup
            for (; idx < accum_count; idx++) {
                local_flat[local_k_idx[idx]] += temp_accum[idx];
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
