#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s343(int iterations,float * flat_2d_array, float aa[256][256], float bb[256][256])
{
    int k;
    for (int nl = 0; nl < 10*(iterations/256); nl++) {
        k = -1;
        for (int i = 0; i < 256; i++) {
            for (int j = 0; j < 256; j++) {
                if (bb[j][i] > (float)0.) {
                    k++;
                    flat_2d_array[k] = aa[j][i];
                }
            }
        }
    }
}

        #include <stdint.h>
#include <string.h>

void vectorized_s343(int iterations, float *flat_2d_array, float aa[256][256], float bb[256][256])
{
    int k;
    for (int nl = 0; nl < 10 * (iterations / 256); nl++) {
        k = -1;
        for (int i = 0; i < 256; i++) {
            /* Process j in chunks of 8 for vector-friendly unrolling */
            int j = 0;
            for (; j <= 256 - 8; j += 8) {
                float b0 = bb[j+0][i], b1 = bb[j+1][i], b2 = bb[j+2][i], b3 = bb[j+3][i];
                float b4 = bb[j+4][i], b5 = bb[j+5][i], b6 = bb[j+6][i], b7 = bb[j+7][i];
                float a0 = aa[j+0][i], a1 = aa[j+1][i], a2 = aa[j+2][i], a3 = aa[j+3][i];
                float a4 = aa[j+4][i], a5 = aa[j+5][i], a6 = aa[j+6][i], a7 = aa[j+7][i];

                int m0 = b0 > 0.0f;
                int m1 = b1 > 0.0f;
                int m2 = b2 > 0.0f;
                int m3 = b3 > 0.0f;
                int m4 = b4 > 0.0f;
                int m5 = b5 > 0.0f;
                int m6 = b6 > 0.0f;
                int m7 = b7 > 0.0f;

                if (m0) { k++; flat_2d_array[k] = a0; }
                if (m1) { k++; flat_2d_array[k] = a1; }
                if (m2) { k++; flat_2d_array[k] = a2; }
                if (m3) { k++; flat_2d_array[k] = a3; }
                if (m4) { k++; flat_2d_array[k] = a4; }
                if (m5) { k++; flat_2d_array[k] = a5; }
                if (m6) { k++; flat_2d_array[k] = a6; }
                if (m7) { k++; flat_2d_array[k] = a7; }
            }
            /* Scalar tail */
            for (; j < 256; j++) {
                if (bb[j][i] > 0.0f) {
                    k++;
                    flat_2d_array[k] = aa[j][i];
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
            int iterations = 5; float flat_2d_array_scalar[128]; float flat_2d_array_vector[128]; float aa_scalar[128]; float aa_vector[128]; float bb_scalar[128]; float bb_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(flat_2d_array_scalar, arr_len, &seed); memcpy(flat_2d_array_vector, flat_2d_array_scalar, sizeof(flat_2d_array_scalar)); fill_f32(aa_scalar, arr_len, &seed); memcpy(aa_vector, aa_scalar, sizeof(aa_scalar)); fill_f32(bb_scalar, arr_len, &seed); memcpy(bb_vector, bb_scalar, sizeof(bb_scalar));
                s343(iterations, flat_2d_array_scalar, aa_scalar, bb_scalar); vectorized_s343(iterations, flat_2d_array_vector, aa_vector, bb_vector);
                for (int i = 0; i < arr_len; ++i) {
    if (fabsf((flat_2d_array_scalar[i]) - (flat_2d_array_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter flat_2d_array on trial %d at index %d\n", trial, i);
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
