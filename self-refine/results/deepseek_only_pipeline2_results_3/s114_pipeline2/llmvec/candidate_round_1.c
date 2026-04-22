#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s114(int iterations, float aa[256][256], float bb[256][256])
{
    int loop_count = 200 * (iterations / 256);

    for (int nl = 0; nl < loop_count; nl++) {
        // Process by diagonal blocks to expose vectorization opportunities
        // Handle all j < i cases where aa[i][j] and aa[j][i] don't alias
        for (int i = 0; i < 256; i++) {
            // Vectorizable part: process in chunks where j and i are far enough apart
            // This ensures aa[i][j] and aa[j][i] are distinct memory locations
            int j;
            // Process with vector-friendly stride
            for (j = 0; j + 8 <= i; j += 8) {
                // Load transposed values first
                float temp0 = aa[j][i];
                float temp1 = aa[j+1][i];
                float temp2 = aa[j+2][i];
                float temp3 = aa[j+3][i];
                float temp4 = aa[j+4][i];
                float temp5 = aa[j+5][i];
                float temp6 = aa[j+6][i];
                float temp7 = aa[j+7][i];

                // Compute and store results
                aa[i][j] = temp0 + bb[i][j];
                aa[i][j+1] = temp1 + bb[i][j+1];
                aa[i][j+2] = temp2 + bb[i][j+2];
                aa[i][j+3] = temp3 + bb[i][j+3];
                aa[i][j+4] = temp4 + bb[i][j+4];
                aa[i][j+5] = temp5 + bb[i][j+5];
                aa[i][j+6] = temp6 + bb[i][j+6];
                aa[i][j+7] = temp7 + bb[i][j+7];
            }
            // Handle remaining elements
            for (; j < i; j++) {
                aa[i][j] = aa[j][i] + bb[i][j];
            }
        }
    }
}

        #include <stdint.h>

void vectorized_s114(int iterations, float aa[256][256], float bb[256][256])
{
    int loop_count = 200 * (iterations / 256);

    for (int nl = 0; nl < loop_count; nl++) {
        for (int i = 0; i < 256; i++) {
            // Vectorized main loop
            int j = 0;
            for (; j + 8 <= i; j += 8) {
                // Load 8 transposed values
                float temp0 = aa[j][i];
                float temp1 = aa[j+1][i];
                float temp2 = aa[j+2][i];
                float temp3 = aa[j+3][i];
                float temp4 = aa[j+4][i];
                float temp5 = aa[j+5][i];
                float temp6 = aa[j+6][i];
                float temp7 = aa[j+7][i];

                // Compute and store 8 results
                aa[i][j] = temp0 + bb[i][j];
                aa[i][j+1] = temp1 + bb[i][j+1];
                aa[i][j+2] = temp2 + bb[i][j+2];
                aa[i][j+3] = temp3 + bb[i][j+3];
                aa[i][j+4] = temp4 + bb[i][j+4];
                aa[i][j+5] = temp5 + bb[i][j+5];
                aa[i][j+6] = temp6 + bb[i][j+6];
                aa[i][j+7] = temp7 + bb[i][j+7];
            }

            // Scalar cleanup for remaining elements
            for (; j < i; j++) {
                aa[i][j] = aa[j][i] + bb[i][j];
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
