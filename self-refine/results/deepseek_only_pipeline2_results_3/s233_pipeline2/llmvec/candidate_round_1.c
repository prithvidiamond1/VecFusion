#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s233(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    for (int nl = 0; nl < 100*(iterations/256); nl++) {
        // Process aa updates first (vertical recurrence in j)
        for (int i = 1; i < 256; i++) {
            for (int j = 1; j < 256; j++) {
                aa[j][i] = aa[j-1][i] + cc[j][i];
            }
        }

        // Process bb updates with original loop order for vectorization in j dimension
        for (int i = 1; i < 256; i++) {
            for (int j = 1; j < 256; j++) {
                bb[j][i] = bb[j][i-1] + cc[j][i];
            }
        }
    }
}

        #include <stdint.h>

void vectorized_s233(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    int nl_iters = 100 * (iterations / 256);

    for (int nl = 0; nl < nl_iters; nl++) {
        // Process aa updates - vertical recurrence in j dimension
        // This has a loop-carried dependency in j, but we can vectorize in i
        for (int i = 1; i < 256; i++) {
            // Vectorize the inner j loop by processing multiple i columns at once
            // Since the recurrence is in j, we keep j as the inner loop
            for (int j = 1; j < 256; j++) {
                aa[j][i] = aa[j-1][i] + cc[j][i];
            }
        }

        // Process bb updates - horizontal recurrence in i dimension
        // This has a loop-carried dependency in i, but we can vectorize in j
        for (int i = 1; i < 256; i++) {
            // Vectorize in j dimension using compiler vector types
            // Process 4 j elements at a time (assuming 128-bit vectors)
            typedef float v4f __attribute__((vector_size(16)));
            int j;

            // Vectorized loop
            for (j = 1; j <= 256 - 4; j += 4) {
                // Load bb[j][i-1] for 4 consecutive j values
                v4f bb_prev = {bb[j][i-1], bb[j+1][i-1], bb[j+2][i-1], bb[j+3][i-1]};

                // Load cc[j][i] for 4 consecutive j values
                v4f cc_vec = {cc[j][i], cc[j+1][i], cc[j+2][i], cc[j+3][i]};

                // Compute bb[j][i] = bb[j][i-1] + cc[j][i]
                v4f result = bb_prev + cc_vec;

                // Store results
                bb[j][i] = result[0];
                bb[j+1][i] = result[1];
                bb[j+2][i] = result[2];
                bb[j+3][i] = result[3];
            }

            // Scalar cleanup for remaining j values
            for (; j < 256; j++) {
                bb[j][i] = bb[j][i-1] + cc[j][i];
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
            int iterations = 5; float aa_scalar[128]; float aa_vector[128]; float bb_scalar[128]; float bb_vector[128]; float cc_scalar[128]; float cc_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(aa_scalar, arr_len, &seed); memcpy(aa_vector, aa_scalar, sizeof(aa_scalar)); fill_f32(bb_scalar, arr_len, &seed); memcpy(bb_vector, bb_scalar, sizeof(bb_scalar)); fill_f32(cc_scalar, arr_len, &seed); memcpy(cc_vector, cc_scalar, sizeof(cc_scalar));
                s233(iterations, aa_scalar, bb_scalar, cc_scalar); vectorized_s233(iterations, aa_vector, bb_vector, cc_vector);
                for (int i = 0; i < arr_len; ++i) {
    if (fabsf((aa_scalar[i]) - (aa_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter aa on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((bb_scalar[i]) - (bb_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter bb on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((cc_scalar[i]) - (cc_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter cc on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
