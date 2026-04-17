#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s114(int iterations, float aa[256][256], float bb[256][256]) {
    int loop_count = 200 * (iterations / 256);

    for (int nl = 0; nl < loop_count; nl++) {
        // Process lower triangular part (j < i)
        for (int i = 1; i < 256; i++) {
            // Copy needed values from upper triangle before modification
            float temp_row[256];
            for (int k = 0; k < i; k++) {
                temp_row[k] = aa[k][i];
            }

            // Compute using temporary values (no dependency within this loop)
            for (int j = 0; j < i; j++) {
                aa[i][j] = temp_row[j] + bb[i][j];
            }
        }
    }
}

        #include <string.h>

void vectorized_s114(int iterations, float aa[256][256], float bb[256][256]) {
    int loop_count = 200 * (iterations / 256);

    for (int nl = 0; nl < loop_count; nl++) {
        for (int i = 1; i < 256; i++) {
            float temp_row[256];

            // Copy needed values from upper triangle before modification
            // temp_row[k] = aa[k][i] for k in [0, i)
            int k = 0;
            for (; k <= i - 4; k += 4) {
                temp_row[k + 0] = aa[k + 0][i];
                temp_row[k + 1] = aa[k + 1][i];
                temp_row[k + 2] = aa[k + 2][i];
                temp_row[k + 3] = aa[k + 3][i];
            }
            for (; k < i; k++) {
                temp_row[k] = aa[k][i];
            }

            // Compute aa[i][j] = temp_row[j] + bb[i][j] for j in [0, i)
            int j = 0;
            for (; j <= i - 8; j += 8) {
                aa[i][j + 0] = temp_row[j + 0] + bb[i][j + 0];
                aa[i][j + 1] = temp_row[j + 1] + bb[i][j + 1];
                aa[i][j + 2] = temp_row[j + 2] + bb[i][j + 2];
                aa[i][j + 3] = temp_row[j + 3] + bb[i][j + 3];
                aa[i][j + 4] = temp_row[j + 4] + bb[i][j + 4];
                aa[i][j + 5] = temp_row[j + 5] + bb[i][j + 5];
                aa[i][j + 6] = temp_row[j + 6] + bb[i][j + 6];
                aa[i][j + 7] = temp_row[j + 7] + bb[i][j + 7];
            }
            for (; j < i; j++) {
                aa[i][j] = temp_row[j] + bb[i][j];
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
            int iterations = 5; float aa_scalar[128]; float aa_vector[128]; float bb_scalar[128]; float bb_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(aa_scalar, arr_len, &seed); memcpy(aa_vector, aa_scalar, sizeof(aa_scalar)); fill_f32(bb_scalar, arr_len, &seed); memcpy(bb_vector, bb_scalar, sizeof(bb_scalar));
                s114(iterations, aa_scalar, bb_scalar); vectorized_s114(iterations, aa_vector, bb_vector);
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
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
