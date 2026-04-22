#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s114(int iterations, float aa[256][256], float bb[256][256]) {
    int loop_count = 200 * (iterations / 256);

    for (int nl = 0; nl < loop_count; nl++) {
        // Temporary array to store computed values for lower triangle
        float temp_lower[256][256];

        // First compute all new values into temporary array
        // This breaks the perceived dependency since we only read from aa
        for (int i = 0; i < 256; i++) {
            int j = 0;
            // Process in chunks of 4 for potential vectorization
            for (; j + 3 < i; j += 4) {
                temp_lower[i][j] = aa[j][i] + bb[i][j];
                temp_lower[i][j+1] = aa[j+1][i] + bb[i][j+1];
                temp_lower[i][j+2] = aa[j+2][i] + bb[i][j+2];
                temp_lower[i][j+3] = aa[j+3][i] + bb[i][j+3];
            }
            // Handle remaining elements
            for (; j < i; j++) {
                temp_lower[i][j] = aa[j][i] + bb[i][j];
            }
        }

        // Then copy back to original array
        // This is a simple contiguous memory copy that can be vectorized
        for (int i = 0; i < 256; i++) {
            int j = 0;
            // Process in chunks of 4 for potential vectorization
            for (; j + 3 < i; j += 4) {
                aa[i][j] = temp_lower[i][j];
                aa[i][j+1] = temp_lower[i][j+1];
                aa[i][j+2] = temp_lower[i][j+2];
                aa[i][j+3] = temp_lower[i][j+3];
            }
            // Handle remaining elements
            for (; j < i; j++) {
                aa[i][j] = temp_lower[i][j];
            }
        }
    }
}

        #include <stddef.h>

void vectorized_s114(int iterations, float aa[256][256], float bb[256][256]) {
    int loop_count = 200 * (iterations / 256);

    for (int nl = 0; nl < loop_count; nl++) {
        float temp_lower[256][256];

        // Compute lower triangle into temp_lower
        for (int i = 0; i < 256; i++) {
            int j = 0;
            // Vectorized chunk processing
            for (; j + 3 < i; j += 4) {
                // Load aa[j:j+3][i] - these are contiguous in memory
                float aa0 = aa[j][i];
                float aa1 = aa[j+1][i];
                float aa2 = aa[j+2][i];
                float aa3 = aa[j+3][i];

                // Load bb[i][j:j+3] - these are contiguous in memory
                float bb0 = bb[i][j];
                float bb1 = bb[i][j+1];
                float bb2 = bb[i][j+2];
                float bb3 = bb[i][j+3];

                // Compute and store
                temp_lower[i][j] = aa0 + bb0;
                temp_lower[i][j+1] = aa1 + bb1;
                temp_lower[i][j+2] = aa2 + bb2;
                temp_lower[i][j+3] = aa3 + bb3;
            }

            // Scalar cleanup for remaining elements
            for (; j < i; j++) {
                temp_lower[i][j] = aa[j][i] + bb[i][j];
            }
        }

        // Copy temp_lower back to aa
        for (int i = 0; i < 256; i++) {
            int j = 0;
            // Vectorized chunk processing
            for (; j + 3 < i; j += 4) {
                // Load temp_lower[i][j:j+3] - contiguous
                float t0 = temp_lower[i][j];
                float t1 = temp_lower[i][j+1];
                float t2 = temp_lower[i][j+2];
                float t3 = temp_lower[i][j+3];

                // Store to aa[i][j:j+3] - contiguous
                aa[i][j] = t0;
                aa[i][j+1] = t1;
                aa[i][j+2] = t2;
                aa[i][j+3] = t3;
            }

            // Scalar cleanup for remaining elements
            for (; j < i; j++) {
                aa[i][j] = temp_lower[i][j];
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
