#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s275(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    for (int nl = 0; nl < 10*(iterations/256); nl++) {
        for (int i = 0; i < 256; i++) {
            if (aa[0][i] > (float)0.) {
                for (int j = 1; j < 256; j++) {
                    aa[j][i] = aa[j-1][i] + bb[j][i] * cc[j][i];
                }
            }
        }
    }
}

        #include <stddef.h>

typedef float v4sf __attribute__((__vector_size__(16)));

void vectorized_s275(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    int total_iterations = 10 * (iterations / 256);
    for (int nl = 0; nl < total_iterations; nl++) {
        for (int i = 0; i < 256; i++) {
            if (aa[0][i] > 0.0f) {
                int j = 1;
                // Vectorized loop: process 4 elements at a time
                for (; j + 4 <= 256; j += 4) {
                    v4sf aa_prev = { aa[j-1][i], aa[j][i], aa[j+1][i], aa[j+2][i] };
                    v4sf bb_vec = { bb[j][i], bb[j+1][i], bb[j+2][i], bb[j+3][i] };
                    v4sf cc_vec = { cc[j][i], cc[j+1][i], cc[j+2][i], cc[j+3][i] };
                    v4sf prod = bb_vec * cc_vec;
                    v4sf aa_new = aa_prev + prod;
                    aa[j][i] = aa_new[0];
                    aa[j+1][i] = aa_new[1];
                    aa[j+2][i] = aa_new[2];
                    aa[j+3][i] = aa_new[3];
                }
                // Scalar cleanup for remaining elements
                for (; j < 256; j++) {
                    aa[j][i] = aa[j-1][i] + bb[j][i] * cc[j][i];
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
            int iterations = 5; float aa_scalar[128]; float aa_vector[128]; float bb_scalar[128]; float bb_vector[128]; float cc_scalar[128]; float cc_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(aa_scalar, arr_len, &seed); memcpy(aa_vector, aa_scalar, sizeof(aa_scalar)); fill_f32(bb_scalar, arr_len, &seed); memcpy(bb_vector, bb_scalar, sizeof(bb_scalar)); fill_f32(cc_scalar, arr_len, &seed); memcpy(cc_vector, cc_scalar, sizeof(cc_scalar));
                s275(iterations, aa_scalar, bb_scalar, cc_scalar); vectorized_s275(iterations, aa_vector, bb_vector, cc_vector);
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
