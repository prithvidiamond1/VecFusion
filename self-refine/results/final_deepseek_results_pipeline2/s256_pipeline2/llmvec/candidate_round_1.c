#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s256(int iterations, float* a, float* d, float aa[256][256], float bb[256][256]) {
    int outer_iters = 10 * (iterations / 256);
    float a_prev[256];

    for (int nl = 0; nl < outer_iters; nl++) {
        // Store initial a values for this nl iteration
        for (int k = 0; k < 256; k++) {
            a_prev[k] = a[k];
        }

        // Process all i for each j to enable vectorization over i
        for (int j = 1; j < 256; j++) {
            // Compute a[j] once per j (independent of i)
            a[j] = (float)1.0 - a[j - 1];

            // Vectorizable loop over i
            for (int i = 0; i < 256; i++) {
                aa[j][i] = a[j] + bb[j][i] * d[j];
            }
        }

        // Restore a[0] for next iteration
        a[0] = a_prev[0];
    }
}

        #include <stdint.h>

void vectorized_s256(int iterations, float* a, float* d, float aa[256][256], float bb[256][256]) {
    int outer_iters = 10 * (iterations / 256);
    float a_prev[256];

    for (int nl = 0; nl < outer_iters; nl++) {
        // Store initial a values
        for (int k = 0; k < 256; k++) {
            a_prev[k] = a[k];
        }

        // Process all i for each j
        for (int j = 1; j < 256; j++) {
            // Compute a[j] once per j
            a[j] = 1.0f - a[j - 1];

            // Vectorizable loop over i with explicit bounds
            float aj = a[j];
            float dj = d[j];
            float* aa_row = aa[j];
            float* bb_row = bb[j];

            // Process 8 elements at a time (256-bit vectors)
            int i = 0;
            for (; i + 7 < 256; i += 8) {
                // Load 8 bb values
                float bb0 = bb_row[i];
                float bb1 = bb_row[i + 1];
                float bb2 = bb_row[i + 2];
                float bb3 = bb_row[i + 3];
                float bb4 = bb_row[i + 4];
                float bb5 = bb_row[i + 5];
                float bb6 = bb_row[i + 6];
                float bb7 = bb_row[i + 7];

                // Compute bb[j][i] * d[j]
                float prod0 = bb0 * dj;
                float prod1 = bb1 * dj;
                float prod2 = bb2 * dj;
                float prod3 = bb3 * dj;
                float prod4 = bb4 * dj;
                float prod5 = bb5 * dj;
                float prod6 = bb6 * dj;
                float prod7 = bb7 * dj;

                // Compute a[j] + bb[j][i] * d[j]
                aa_row[i] = aj + prod0;
                aa_row[i + 1] = aj + prod1;
                aa_row[i + 2] = aj + prod2;
                aa_row[i + 3] = aj + prod3;
                aa_row[i + 4] = aj + prod4;
                aa_row[i + 5] = aj + prod5;
                aa_row[i + 6] = aj + prod6;
                aa_row[i + 7] = aj + prod7;
            }

            // Scalar cleanup for remaining elements
            for (; i < 256; i++) {
                aa_row[i] = aj + bb_row[i] * dj;
            }
        }

        // Restore a[0] for next iteration
        a[0] = a_prev[0];
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
            int iterations = 5; float a_scalar[128]; float a_vector[128]; float d_scalar[128]; float d_vector[128]; float aa_scalar[128]; float aa_vector[128]; float bb_scalar[128]; float bb_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(d_scalar, arr_len, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar)); fill_f32(aa_scalar, arr_len, &seed); memcpy(aa_vector, aa_scalar, sizeof(aa_scalar)); fill_f32(bb_scalar, arr_len, &seed); memcpy(bb_vector, bb_scalar, sizeof(bb_scalar));
                s256(iterations, a_scalar, d_scalar, aa_scalar, bb_scalar); vectorized_s256(iterations, a_vector, d_vector, aa_vector, bb_vector);
                for (int i = 0; i < arr_len; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((d_scalar[i]) - (d_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter d on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
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
