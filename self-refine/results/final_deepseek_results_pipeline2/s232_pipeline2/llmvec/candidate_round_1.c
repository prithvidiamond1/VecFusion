#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s232(int iterations, float aa[256][256], float bb[256][256])
{
    int loop_count = 100 * (iterations / 256);

    for (int nl = 0; nl < loop_count; nl++) {
        for (int j = 1; j < 256; j++) {
            // Store the previous value to break dependency chain
            float prev = aa[j][0];

            for (int i = 1; i <= j; i++) {
                // Compute using stored previous value
                float current = prev * prev + bb[j][i];
                // Update array and store for next iteration
                aa[j][i] = current;
                prev = current;
            }
        }
    }
}

        #include <stdint.h>

void vectorized_s232(int iterations, float aa[256][256], float bb[256][256])
{
    int loop_count = 100 * (iterations / 256);

    for (int nl = 0; nl < loop_count; nl++) {
        for (int j = 1; j < 256; j++) {
            // Process vectorizable portion with explicit bounds
            int i = 1;
            // Vectorize inner loop where possible
            for (; i + 3 <= j; i += 4) {
                // Load previous values (scalar expansion for vectorization)
                float prev0 = aa[j][i-1];
                float prev1 = prev0 * prev0 + bb[j][i];
                float prev2 = prev1 * prev1 + bb[j][i+1];
                float prev3 = prev2 * prev2 + bb[j][i+2];
                float prev4 = prev3 * prev3 + bb[j][i+3];

                // Store results
                aa[j][i] = prev1;
                aa[j][i+1] = prev2;
                aa[j][i+2] = prev3;
                aa[j][i+3] = prev4;

                // Update prev for next iteration
                aa[j][i-1] = prev0; // Already stored from previous iteration
            }

            // Scalar cleanup for remaining iterations
            float prev = aa[j][i-1];
            for (; i <= j; i++) {
                float current = prev * prev + bb[j][i];
                aa[j][i] = current;
                prev = current;
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
