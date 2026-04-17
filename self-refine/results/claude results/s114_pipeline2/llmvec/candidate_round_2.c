#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s114(int iterations, float aa[256][256], float bb[256][256])
{
    float trans[256][256];
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < i; j++) {
            trans[i][j] = aa[j][i];
        }
    }

    for (int nl = 0; nl < 200*(iterations/(256)); nl++) {
        for (int i = 1; i < 256; i++) {
            for (int j = 0; j < i; j++) {
                aa[i][j] = trans[i][j] + bb[i][j];
            }
        }
        for (int i = 0; i < 256; i++) {
            for (int j = 0; j < i; j++) {
                trans[i][j] = aa[j][i];
            }
        }
    }
}

        #include <string.h>
#include <stdlib.h>

void vectorized_s114(int iterations, float aa[256][256], float bb[256][256])
{
    static float trans[256][256];

    // Initialize trans: trans[i][j] = aa[j][i] for j < i
    for (int i = 0; i < 256; i++) {
        int j = 0;
        for (; j + 7 < i; j += 8) {
            trans[i][j+0] = aa[j+0][i];
            trans[i][j+1] = aa[j+1][i];
            trans[i][j+2] = aa[j+2][i];
            trans[i][j+3] = aa[j+3][i];
            trans[i][j+4] = aa[j+4][i];
            trans[i][j+5] = aa[j+5][i];
            trans[i][j+6] = aa[j+6][i];
            trans[i][j+7] = aa[j+7][i];
        }
        for (; j < i; j++) {
            trans[i][j] = aa[j][i];
        }
    }

    int limit = 200 * (iterations / 256);

    for (int nl = 0; nl < limit; nl++) {
        // aa[i][j] = trans[i][j] + bb[i][j] for j < i, i from 1..255
        for (int i = 1; i < 256; i++) {
            int j = 0;
            for (; j + 7 < i; j += 8) {
                aa[i][j+0] = trans[i][j+0] + bb[i][j+0];
                aa[i][j+1] = trans[i][j+1] + bb[i][j+1];
                aa[i][j+2] = trans[i][j+2] + bb[i][j+2];
                aa[i][j+3] = trans[i][j+3] + bb[i][j+3];
                aa[i][j+4] = trans[i][j+4] + bb[i][j+4];
                aa[i][j+5] = trans[i][j+5] + bb[i][j+5];
                aa[i][j+6] = trans[i][j+6] + bb[i][j+6];
                aa[i][j+7] = trans[i][j+7] + bb[i][j+7];
            }
            for (; j < i; j++) {
                aa[i][j] = trans[i][j] + bb[i][j];
            }
        }

        // trans[i][j] = aa[j][i] for j < i
        for (int i = 0; i < 256; i++) {
            int j = 0;
            for (; j + 7 < i; j += 8) {
                trans[i][j+0] = aa[j+0][i];
                trans[i][j+1] = aa[j+1][i];
                trans[i][j+2] = aa[j+2][i];
                trans[i][j+3] = aa[j+3][i];
                trans[i][j+4] = aa[j+4][i];
                trans[i][j+5] = aa[j+5][i];
                trans[i][j+6] = aa[j+6][i];
                trans[i][j+7] = aa[j+7][i];
            }
            for (; j < i; j++) {
                trans[i][j] = aa[j][i];
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
            const int n = 128;
            uint32_t seed = 7u;
            int iterations = 5; float aa_scalar[128]; float aa_vector[128]; float bb_scalar[128]; float bb_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(aa_scalar, n, &seed); memcpy(aa_vector, aa_scalar, sizeof(aa_scalar)); fill_f32(bb_scalar, n, &seed); memcpy(bb_vector, bb_scalar, sizeof(bb_scalar));
                s114(iterations, aa_scalar, bb_scalar); vectorized_s114(iterations, aa_vector, bb_vector);
                for (int i = 0; i < n; ++i) {
    if (fabsf((aa_scalar[i]) - (aa_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter aa on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((bb_scalar[i]) - (bb_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter bb on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
