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
typedef float float32x4_t __attribute__((__vector_size__(16)));

void vectorized_s256(int iterations, float* a, float* d, float aa[256][256], float bb[256][256])
{
    int total_iters = 10 * (iterations / 256);
    for (int nl = 0; nl < total_iters; nl++) {
        for (int i = 0; i < 256; i++) {
            // Scalar start for j=0 (no previous a[-1])
            a[0] = 1.0f - a[0];
            aa[0][i] = a[0] + bb[0][i] * d[0];

            // Vectorized loop for j = 1 to 255, processing 4 elements at a time
            int j;
            for (j = 1; j <= 252; j += 4) {
                float32x4_t a_prev = { a[j-1], a[j], a[j+1], a[j+2] };
                float32x4_t a_new = 1.0f - a_prev;
                a[j] = a_new[0];
                a[j+1] = a_new[1];
                a[j+2] = a_new[2];
                a[j+3] = a_new[3];

                float32x4_t bb_vals = { bb[j][i], bb[j+1][i], bb[j+2][i], bb[j+3][i] };
                float32x4_t d_vals = { d[j], d[j+1], d[j+2], d[j+3] };
                float32x4_t aa_vals = a_new + bb_vals * d_vals;
                aa[j][i] = aa_vals[0];
                aa[j+1][i] = aa_vals[1];
                aa[j+2][i] = aa_vals[2];
                aa[j+3][i] = aa_vals[3];
            }

            // Scalar cleanup for remaining elements
            for (; j < 256; j++) {
                a[j] = 1.0f - a[j-1];
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
