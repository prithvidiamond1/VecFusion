#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s256(int iterations, float* a, float* d, float aa[256][256], float bb[256][256])
{
    int loop_count = 10 * (iterations / 256);

    for (int nl = 0; nl < loop_count; nl++) {
        // Precompute all a[j] values using closed-form solution
        float a0 = a[0];
        float a_temp[256];
        a_temp[0] = a0;

        // Vectorizable loop: compute a[j] = (j % 2 == 1) ? (1.0f - a0) : a0
        for (int j = 1; j < 256; j++) {
            a_temp[j] = (j & 1) ? (1.0f - a0) : a0;
        }

        // Update original a array
        for (int j = 1; j < 256; j++) {
            a[j] = a_temp[j];
        }

        // Compute aa[j][i] with i-loop outermost for better locality
        for (int i = 0; i < 256; i++) {
            for (int j = 1; j < 256; j++) {
                aa[j][i] = a_temp[j] + bb[j][i] * d[j];
            }
        }
    }
}

        #include <stdint.h>

void vectorized_s256(int iterations, float* a, float* d, float aa[256][256], float bb[256][256])
{
    int loop_count = 10 * (iterations / 256);

    for (int nl = 0; nl < loop_count; nl++) {
        float a0 = a[0];
        float a_temp[256];
        a_temp[0] = a0;

        // Vectorized computation of a_temp[j]
        for (int j = 1; j < 256; j += 4) {
            // Create pattern: [a0, 1-a0, a0, 1-a0] for consecutive j values
            float vec_a0[4] = {a0, a0, a0, a0};
            float vec_one_minus_a0[4] = {1.0f - a0, 1.0f - a0, 1.0f - a0, 1.0f - a0};

            // Pattern based on j & 1
            int pattern[4] = {1 & 1, (j+1) & 1, (j+2) & 1, (j+3) & 1};

            for (int k = 0; k < 4; k++) {
                a_temp[j + k] = pattern[k] ? vec_one_minus_a0[k] : vec_a0[k];
            }
        }

        // Update original a array
        for (int j = 1; j < 256; j++) {
            a[j] = a_temp[j];
        }

        // Vectorized computation of aa[j][i]
        for (int i = 0; i < 256; i++) {
            for (int j = 1; j < 256; j += 4) {
                // Process 4 j values at a time
                for (int k = 0; k < 4 && (j + k) < 256; k++) {
                    aa[j + k][i] = a_temp[j + k] + bb[j + k][i] * d[j + k];
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
