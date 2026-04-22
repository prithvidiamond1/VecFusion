#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s235(int iterations, float* a, float* b, float* c, float aa[256][256], float bb[256][256])
{
    int total_nl = 200 * (iterations / 256);

    for (int nl = 0; nl < total_nl; nl++) {
        // First: compute all a[i] updates
        for (int i = 0; i < 256; i++) {
            a[i] += b[i] * c[i];
        }

        // Second: compute aa columns independently
        for (int i = 0; i < 256; i++) {
            float a_val = a[i];  // Store in register for reuse
            float aa_prev = aa[0][i];  // Store previous value

            for (int j = 1; j < 256; j++) {
                aa_prev = aa_prev + bb[j][i] * a_val;
                aa[j][i] = aa_prev;
            }
        }
    }
}

        #include <stdint.h>

void vectorized_s235(int iterations, float* a, float* b, float* c, float aa[256][256], float bb[256][256])
{
    int total_nl = 200 * (iterations / 256);

    for (int nl = 0; nl < total_nl; nl++) {
        // First: compute all a[i] updates - vectorized with unrolling
        int i = 0;
        for (; i <= 256 - 8; i += 8) {
            a[i+0] += b[i+0] * c[i+0];
            a[i+1] += b[i+1] * c[i+1];
            a[i+2] += b[i+2] * c[i+2];
            a[i+3] += b[i+3] * c[i+3];
            a[i+4] += b[i+4] * c[i+4];
            a[i+5] += b[i+5] * c[i+5];
            a[i+6] += b[i+6] * c[i+6];
            a[i+7] += b[i+7] * c[i+7];
        }
        for (; i < 256; i++) {
            a[i] += b[i] * c[i];
        }

        // Second: compute aa columns independently
        // Process multiple columns at once (unroll over i)
        int col = 0;
        for (; col <= 256 - 4; col += 4) {
            float a_val0 = a[col+0];
            float a_val1 = a[col+1];
            float a_val2 = a[col+2];
            float a_val3 = a[col+3];

            float aa_prev0 = aa[0][col+0];
            float aa_prev1 = aa[0][col+1];
            float aa_prev2 = aa[0][col+2];
            float aa_prev3 = aa[0][col+3];

            for (int j = 1; j < 256; j++) {
                aa_prev0 = aa_prev0 + bb[j][col+0] * a_val0;
                aa_prev1 = aa_prev1 + bb[j][col+1] * a_val1;
                aa_prev2 = aa_prev2 + bb[j][col+2] * a_val2;
                aa_prev3 = aa_prev3 + bb[j][col+3] * a_val3;
                aa[j][col+0] = aa_prev0;
                aa[j][col+1] = aa_prev1;
                aa[j][col+2] = aa_prev2;
                aa[j][col+3] = aa_prev3;
            }
        }
        for (; col < 256; col++) {
            float a_val = a[col];
            float aa_prev = aa[0][col];
            for (int j = 1; j < 256; j++) {
                aa_prev = aa_prev + bb[j][col] * a_val;
                aa[j][col] = aa_prev;
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
            int iterations = 5; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float aa_scalar[128]; float aa_vector[128]; float bb_scalar[128]; float bb_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(aa_scalar, arr_len, &seed); memcpy(aa_vector, aa_scalar, sizeof(aa_scalar)); fill_f32(bb_scalar, arr_len, &seed); memcpy(bb_vector, bb_scalar, sizeof(bb_scalar));
                s235(iterations, a_scalar, b_scalar, c_scalar, aa_scalar, bb_scalar); vectorized_s235(iterations, a_vector, b_vector, c_vector, aa_vector, bb_vector);
                for (int i = 0; i < arr_len; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((b_scalar[i]) - (b_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter b on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((c_scalar[i]) - (c_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter c on trial %d at index %d\n", trial, i);
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
