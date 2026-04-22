#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s231(int iterations, float aa[256][256], float bb[256][256]) {
    int loop_count = 100 * (iterations / 256);
    for (int nl = 0; nl < loop_count; nl++) {
        for (int j = 1; j < 256; j++) {
            for (int i = 0; i < 256; i += 4) {
                // Vector-friendly: process 4 i-elements at a time
                float prev0 = aa[j-1][i];
                float prev1 = aa[j-1][i+1];
                float prev2 = aa[j-1][i+2];
                float prev3 = aa[j-1][i+3];

                aa[j][i]   = prev0 + bb[j][i];
                aa[j][i+1] = prev1 + bb[j][i+1];
                aa[j][i+2] = prev2 + bb[j][i+2];
                aa[j][i+3] = prev3 + bb[j][i+3];
            }
        }
    }
}

        #include <string.h>

typedef float v4f __attribute__((vector_size(16)));

void vectorized_s231(int iterations, float aa[256][256], float bb[256][256]) {
    int loop_count = 100 * (iterations / 256);
    for (int nl = 0; nl < loop_count; nl++) {
        for (int j = 1; j < 256; j++) {
            int i = 0;
            for (; i <= 256 - 16; i += 16) {
                v4f prev0, prev1, prev2, prev3;
                v4f bv0, bv1, bv2, bv3;

                __builtin_memcpy(&prev0, &aa[j-1][i],    sizeof(v4f));
                __builtin_memcpy(&prev1, &aa[j-1][i+4],  sizeof(v4f));
                __builtin_memcpy(&prev2, &aa[j-1][i+8],  sizeof(v4f));
                __builtin_memcpy(&prev3, &aa[j-1][i+12], sizeof(v4f));

                __builtin_memcpy(&bv0, &bb[j][i],    sizeof(v4f));
                __builtin_memcpy(&bv1, &bb[j][i+4],  sizeof(v4f));
                __builtin_memcpy(&bv2, &bb[j][i+8],  sizeof(v4f));
                __builtin_memcpy(&bv3, &bb[j][i+12], sizeof(v4f));

                v4f res0 = prev0 + bv0;
                v4f res1 = prev1 + bv1;
                v4f res2 = prev2 + bv2;
                v4f res3 = prev3 + bv3;

                __builtin_memcpy(&aa[j][i],    &res0, sizeof(v4f));
                __builtin_memcpy(&aa[j][i+4],  &res1, sizeof(v4f));
                __builtin_memcpy(&aa[j][i+8],  &res2, sizeof(v4f));
                __builtin_memcpy(&aa[j][i+12], &res3, sizeof(v4f));
            }
            for (; i <= 256 - 4; i += 4) {
                v4f prev, bv;
                __builtin_memcpy(&prev, &aa[j-1][i], sizeof(v4f));
                __builtin_memcpy(&bv,   &bb[j][i],   sizeof(v4f));
                v4f res = prev + bv;
                __builtin_memcpy(&aa[j][i], &res, sizeof(v4f));
            }
            for (; i < 256; i++) {
                aa[j][i] = aa[j-1][i] + bb[j][i];
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
                s231(iterations, aa_scalar, bb_scalar); vectorized_s231(iterations, aa_vector, bb_vector);
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
