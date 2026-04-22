#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s232(int iterations, float aa[256][256], float bb[256][256]) {
    int outer_iters = 100 * (iterations / 256);

    for (int nl = 0; nl < outer_iters; nl++) {
        for (int j = 1; j < 256; j++) {
            // Process first element separately
            float prev = aa[j][0] * aa[j][0] + bb[j][1];
            aa[j][1] = prev;

            // Main loop with explicit scalar temporary
            for (int i = 2; i <= j; i++) {
                prev = prev * prev + bb[j][i];
                aa[j][i] = prev;
            }
        }
    }
}

        void vectorized_s232(int iterations, float aa[256][256], float bb[256][256]) {
    int outer_iters = 100 * (iterations / 256);

    for (int nl = 0; nl < outer_iters; nl++) {
        // Process j in groups of 4 for ILP, but each j has different bounds
        // so we handle the triangular recurrence per-j
        int j = 1;
        for (; j <= 252; j += 4) {
            int j0 = j, j1 = j+1, j2 = j+2, j3 = j+3;

            float prev0 = aa[j0][0] * aa[j0][0] + bb[j0][1];
            float prev1 = aa[j1][0] * aa[j1][0] + bb[j1][1];
            float prev2 = aa[j2][0] * aa[j2][0] + bb[j2][1];
            float prev3 = aa[j3][0] * aa[j3][0] + bb[j3][1];

            aa[j0][1] = prev0;
            aa[j1][1] = prev1;
            aa[j2][1] = prev2;
            aa[j3][1] = prev3;

            // j0 loop: i from 2 to j0
            for (int i = 2; i <= j0; i++) {
                prev0 = prev0 * prev0 + bb[j0][i];
                aa[j0][i] = prev0;
            }
            // j1 loop: i from 2 to j1
            // reuse prev0 result for i <= j0, then continue
            // Actually each j is independent, reset prev from aa
            prev1 = aa[j1][1];
            for (int i = 2; i <= j1; i++) {
                prev1 = prev1 * prev1 + bb[j1][i];
                aa[j1][i] = prev1;
            }
            prev2 = aa[j2][1];
            for (int i = 2; i <= j2; i++) {
                prev2 = prev2 * prev2 + bb[j2][i];
                aa[j2][i] = prev2;
            }
            prev3 = aa[j3][1];
            for (int i = 2; i <= j3; i++) {
                prev3 = prev3 * prev3 + bb[j3][i];
                aa[j3][i] = prev3;
            }
        }
        // Scalar cleanup for remaining j values
        for (; j < 256; j++) {
            float prev = aa[j][0] * aa[j][0] + bb[j][1];
            aa[j][1] = prev;
            for (int i = 2; i <= j; i++) {
                prev = prev * prev + bb[j][i];
                aa[j][i] = prev;
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
                s232(iterations, aa_scalar, bb_scalar); vectorized_s232(iterations, aa_vector, bb_vector);
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
