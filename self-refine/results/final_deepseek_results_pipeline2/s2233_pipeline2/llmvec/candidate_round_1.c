#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s2233(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    int loop_count = 100 * (iterations / 256);

    for (int nl = 0; nl < loop_count; nl++) {
        // Process aa updates column by column
        for (int i = 1; i < 256; i++) {
            for (int j = 1; j < 256; j++) {
                aa[j][i] = aa[j-1][i] + cc[j][i];
            }
        }

        // Process bb updates row by row
        for (int i = 1; i < 256; i++) {
            for (int j = 1; j < 256; j++) {
                bb[i][j] = bb[i-1][j] + cc[i][j];
            }
        }
    }
}

        #include <stdint.h>

void vectorized_s2233(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    int loop_count = 100 * (iterations / 256);

    for (int nl = 0; nl < loop_count; nl++) {
        // Process aa updates column by column
        for (int i = 1; i < 256; i++) {
            // Vectorize the inner j loop
            int j = 1;
            // Process in chunks of 4
            for (; j + 3 < 256; j += 4) {
                // Load previous row values
                float prev0 = aa[j-1][i];
                float prev1 = aa[j][i];
                float prev2 = aa[j+1][i];
                float prev3 = aa[j+2][i];

                // Load cc values
                float cc0 = cc[j][i];
                float cc1 = cc[j+1][i];
                float cc2 = cc[j+2][i];
                float cc3 = cc[j+3][i];

                // Compute new values
                aa[j][i] = prev0 + cc0;
                aa[j+1][i] = prev1 + cc1;
                aa[j+2][i] = prev2 + cc2;
                aa[j+3][i] = prev3 + cc3;
            }
            // Scalar tail
            for (; j < 256; j++) {
                aa[j][i] = aa[j-1][i] + cc[j][i];
            }
        }

        // Process bb updates row by row
        for (int i = 1; i < 256; i++) {
            // Vectorize the inner j loop
            int j = 1;
            // Process in chunks of 4
            for (; j + 3 < 256; j += 4) {
                // Load previous column values
                float prev0 = bb[i-1][j];
                float prev1 = bb[i-1][j+1];
                float prev2 = bb[i-1][j+2];
                float prev3 = bb[i-1][j+3];

                // Load cc values
                float cc0 = cc[i][j];
                float cc1 = cc[i][j+1];
                float cc2 = cc[i][j+2];
                float cc3 = cc[i][j+3];

                // Compute new values
                bb[i][j] = prev0 + cc0;
                bb[i][j+1] = prev1 + cc1;
                bb[i][j+2] = prev2 + cc2;
                bb[i][j+3] = prev3 + cc3;
            }
            // Scalar tail
            for (; j < 256; j++) {
                bb[i][j] = bb[i-1][j] + cc[i][j];
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
                s2233(iterations, aa_scalar, bb_scalar, cc_scalar); vectorized_s2233(iterations, aa_vector, bb_vector, cc_vector);
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
