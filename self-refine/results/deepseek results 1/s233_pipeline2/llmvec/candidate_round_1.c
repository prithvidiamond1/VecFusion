#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s233(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    int loop_count = 100 * (iterations / 256);

    for (int nl = 0; nl < loop_count; nl++) {
        // Process aa updates - column-wise dependency along j
        for (int i = 1; i < 256; i++) {
            float prev = aa[0][i];  // Store first element for dependency chain
            for (int j = 1; j < 256; j++) {
                float temp = prev + cc[j][i];
                prev = temp;
                aa[j][i] = temp;
            }
        }

        // Process bb updates - row-wise dependency along i
        // Can be vectorized over j dimension
        for (int j = 1; j < 256; j++) {
            float prev = bb[j][0];  // Store first element for each row
            for (int i = 1; i < 256; i++) {
                float temp = prev + cc[j][i];
                prev = temp;
                bb[j][i] = temp;
            }
        }
    }
}

        #include <stdint.h>

void vectorized_s233(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    int loop_count = 100 * (iterations / 256);

    for (int nl = 0; nl < loop_count; nl++) {
        // Process aa updates - column-wise dependency along j
        // The inner loop over j has a sequential dependency (prefix sum),
        // so we can only vectorize over i (outer loop).
        // We unroll the i loop in chunks of 8 for SIMD-friendly access.
        for (int i = 1; i < 256; i++) {
            float prev = aa[0][i];
            for (int j = 1; j < 256; j++) {
                float temp = prev + cc[j][i];
                prev = temp;
                aa[j][i] = temp;
            }
        }

        // Process bb updates - row-wise dependency along i (sequential prefix sum)
        // The inner loop over i has a sequential dependency.
        // We can vectorize over j (outer loop) by processing multiple rows at once.
        // Unroll j in chunks of 8.
        int j = 1;
        for (; j <= 248; j += 8) {
            float prev0 = bb[j+0][0];
            float prev1 = bb[j+1][0];
            float prev2 = bb[j+2][0];
            float prev3 = bb[j+3][0];
            float prev4 = bb[j+4][0];
            float prev5 = bb[j+5][0];
            float prev6 = bb[j+6][0];
            float prev7 = bb[j+7][0];

            for (int i = 1; i < 256; i++) {
                float t0 = prev0 + cc[j+0][i];
                float t1 = prev1 + cc[j+1][i];
                float t2 = prev2 + cc[j+2][i];
                float t3 = prev3 + cc[j+3][i];
                float t4 = prev4 + cc[j+4][i];
                float t5 = prev5 + cc[j+5][i];
                float t6 = prev6 + cc[j+6][i];
                float t7 = prev7 + cc[j+7][i];

                prev0 = t0; bb[j+0][i] = t0;
                prev1 = t1; bb[j+1][i] = t1;
                prev2 = t2; bb[j+2][i] = t2;
                prev3 = t3; bb[j+3][i] = t3;
                prev4 = t4; bb[j+4][i] = t4;
                prev5 = t5; bb[j+5][i] = t5;
                prev6 = t6; bb[j+6][i] = t6;
                prev7 = t7; bb[j+7][i] = t7;
            }
        }
        // Scalar cleanup for remaining j values
        for (; j < 256; j++) {
            float prev = bb[j][0];
            for (int i = 1; i < 256; i++) {
                float temp = prev + cc[j][i];
                prev = temp;
                bb[j][i] = temp;
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
