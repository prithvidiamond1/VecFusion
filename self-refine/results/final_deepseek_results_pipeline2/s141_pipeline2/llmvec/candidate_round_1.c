#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s141(int iterations, float * flat_2d_array, float bb[256][256]) {
    int nl_iter = 200 * (iterations / 256);

    for (int nl = 0; nl < nl_iter; nl++) {
        // Precompute all k values for each i (starting point for each row)
        int k_start[256];
        for (int i = 0; i < 256; i++) {
            k_start[i] = (i+1) * i / 2 + i;
        }

        // Process by j first to enable vectorization on i
        // This creates regular access pattern: bb[j][i] for i=0..j
        for (int j = 0; j < 256; j++) {
            // For each j, process all i <= j
            // Compute k incrementally for each i
            int k = k_start[0] + j;  // k for i=0, j=j
            for (int i = 0; i <= j; i++) {
                flat_2d_array[k] += bb[j][i];
                // Update k for next i: k(i+1,j) = k(i,j) + (j+1) - (i+1)
                if (i < j) {
                    k += (j - i);
                }
            }
        }
    }
}

        #include <stddef.h>

void vectorized_s141(int iterations, float * flat_2d_array, float bb[256][256]) {
    int nl_iter = 200 * (iterations / 256);

    for (int nl = 0; nl < nl_iter; nl++) {
        // Precompute k_start values for each row i
        int k_start[256];
        for (int i = 0; i < 256; i++) {
            k_start[i] = (i + 1) * i / 2 + i;
        }

        // Process by j first for regular access pattern
        for (int j = 0; j < 256; j++) {
            // Compute initial k for i=0
            int k = k_start[0] + j;

            // Process i from 0 to j inclusive
            for (int i = 0; i <= j; i++) {
                flat_2d_array[k] += bb[j][i];

                // Update k for next i if not at last iteration
                if (i < j) {
                    k += (j - i);
                }
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
