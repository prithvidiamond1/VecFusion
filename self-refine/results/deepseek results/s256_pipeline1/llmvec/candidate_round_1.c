#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s256(int iterations, float* a,float* d, float aa[256][256],float bb[256][256])
{
    for (int nl = 0; nl < 10*(iterations/256); nl++) {
        for (int i = 0; i < 256; i++) {
            for (int j = 1; j < 256; j++) {
                a[j] = (float)1.0 - a[j - 1];
                aa[j][i] = a[j] + bb[j][i]*d[j];
            }
        }
    }
}

        #include <stdint.h>

void vectorized_s256(int iterations, float* a, float* d, float aa[256][256], float bb[256][256])
{
    for (int nl = 0; nl < 10*(iterations/256); nl++) {
        for (int i = 0; i < 256; i++) {
            // j=1: a[1] = 1.0f - a[0]
            // j=2: a[2] = 1.0f - a[1]
            // Each iteration depends on the previous a[j-1], so this is a
            // recurrence. We cannot vectorize the a[j] computation directly,
            // but we can observe the pattern:
            // a[1] = 1.0f - a[0]
            // a[2] = 1.0f - a[1] = 1.0f - (1.0f - a[0]) = a[0]
            // a[3] = 1.0f - a[2] = 1.0f - a[0]
            // a[4] = 1.0f - a[3] = a[0]
            // So: a[j] = (j is odd) ? (1.0f - a[0]) : a[0]  for j >= 1
            // We can use this to vectorize the aa computation.

            float a0 = a[0];
            float a_odd  = 1.0f - a0;  // a[j] when j is odd
            float a_even = a0;          // a[j] when j is even (j>=2)

            // Fill a[] array first (scalar, but simple)
            for (int j = 1; j < 256; j++) {
                a[j] = (j & 1) ? a_odd : a_even;
            }

            // Now vectorize the aa computation using unrolling
            // aa[j][i] = a[j] + bb[j][i] * d[j]
            // Unroll by 8
            int j = 1;
            for (; j <= 255 - 7; j += 8) {
                aa[j+0][i] = a[j+0] + bb[j+0][i] * d[j+0];
                aa[j+1][i] = a[j+1] + bb[j+1][i] * d[j+1];
                aa[j+2][i] = a[j+2] + bb[j+2][i] * d[j+2];
                aa[j+3][i] = a[j+3] + bb[j+3][i] * d[j+3];
                aa[j+4][i] = a[j+4] + bb[j+4][i] * d[j+4];
                aa[j+5][i] = a[j+5] + bb[j+5][i] * d[j+5];
                aa[j+6][i] = a[j+6] + bb[j+6][i] * d[j+6];
                aa[j+7][i] = a[j+7] + bb[j+7][i] * d[j+7];
            }
            for (; j < 256; j++) {
                aa[j][i] = a[j] + bb[j][i] * d[j];
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
