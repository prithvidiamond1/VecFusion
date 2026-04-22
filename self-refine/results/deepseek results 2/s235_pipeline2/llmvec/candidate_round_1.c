#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s235(int iterations, float* a, float* b, float* c, float aa[256][256], float bb[256][256]) {
    for (int nl = 0; nl < 200 * (iterations / 256); nl++) {
        // Loop distribution: separate a[i] update from aa computation
        for (int i = 0; i < 256; i++) {
            a[i] += b[i] * c[i];
        }

        // Loop reordering: j outer, i inner with tiling for cache locality
        const int TILE_SIZE = 64;
        for (int i0 = 0; i0 < 256; i0 += TILE_SIZE) {
            int i_end = i0 + TILE_SIZE;
            if (i_end > 256) i_end = 256;

            for (int j = 1; j < 256; j++) {
                for (int i = i0; i < i_end; i++) {
                    aa[j][i] = aa[j-1][i] + bb[j][i] * a[i];
                }
            }
        }
    }
}

        #include <stdint.h>

void vectorized_s235(int iterations, float* a, float* b, float* c, float aa[256][256], float bb[256][256]) {
    for (int nl = 0; nl < 200 * (iterations / 256); nl++) {
        // Vectorized a[i] += b[i] * c[i]
        int i;
        for (i = 0; i <= 256 - 8; i += 8) {
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

        // Tiled loop with vectorized inner loop
        const int TILE_SIZE = 64;
        for (int i0 = 0; i0 < 256; i0 += TILE_SIZE) {
            int i_end = i0 + TILE_SIZE;
            if (i_end > 256) i_end = 256;

            for (int j = 1; j < 256; j++) {
                int ii;
                for (ii = i0; ii <= i_end - 8; ii += 8) {
                    aa[j][ii+0] = aa[j-1][ii+0] + bb[j][ii+0] * a[ii+0];
                    aa[j][ii+1] = aa[j-1][ii+1] + bb[j][ii+1] * a[ii+1];
                    aa[j][ii+2] = aa[j-1][ii+2] + bb[j][ii+2] * a[ii+2];
                    aa[j][ii+3] = aa[j-1][ii+3] + bb[j][ii+3] * a[ii+3];
                    aa[j][ii+4] = aa[j-1][ii+4] + bb[j][ii+4] * a[ii+4];
                    aa[j][ii+5] = aa[j-1][ii+5] + bb[j][ii+5] * a[ii+5];
                    aa[j][ii+6] = aa[j-1][ii+6] + bb[j][ii+6] * a[ii+6];
                    aa[j][ii+7] = aa[j-1][ii+7] + bb[j][ii+7] * a[ii+7];
                }
                for (; ii < i_end; ii++) {
                    aa[j][ii] = aa[j-1][ii] + bb[j][ii] * a[ii];
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
