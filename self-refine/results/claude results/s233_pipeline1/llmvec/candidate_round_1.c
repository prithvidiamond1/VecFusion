#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s233(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    for (int nl = 0; nl < 100*(iterations/256); nl++) {
        for (int i = 1; i < 256; i++) {
            for (int j = 1; j < 256; j++) {
                aa[j][i] = aa[j-1][i] + cc[j][i];
            }
            for (int j = 1; j < 256; j++) {
                bb[j][i] = bb[j][i-1] + cc[j][i];
            }
        }
    }
}

        #include <stdint.h>

void vectorized_s233(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    for (int nl = 0; nl < 100*(iterations/256); nl++) {
        for (int i = 1; i < 256; i++) {
            // First loop: aa[j][i] = aa[j-1][i] + cc[j][i]
            // This is a sequential dependency on j, so we process scalar
            // but unroll to help the compiler
            int j = 1;
            for (; j <= 252; j += 4) {
                aa[j][i]   = aa[j-1][i]   + cc[j][i];
                aa[j+1][i] = aa[j][i]     + cc[j+1][i];
                aa[j+2][i] = aa[j+1][i]   + cc[j+2][i];
                aa[j+3][i] = aa[j+2][i]   + cc[j+3][i];
            }
            for (; j < 256; j++) {
                aa[j][i] = aa[j-1][i] + cc[j][i];
            }

            // Second loop: bb[j][i] = bb[j][i-1] + cc[j][i]
            // No dependency on j, so we can vectorize across j
            j = 1;
#if defined(__clang__) || defined(__GNUC__)
            typedef float float4 __attribute__((vector_size(16)));
            for (; j <= 252; j += 4) {
                float4 bv, cv;
                // Load bb[j..j+3][i-1]
                bv[0] = bb[j][i-1];
                bv[1] = bb[j+1][i-1];
                bv[2] = bb[j+2][i-1];
                bv[3] = bb[j+3][i-1];
                // Load cc[j..j+3][i]
                cv[0] = cc[j][i];
                cv[1] = cc[j+1][i];
                cv[2] = cc[j+2][i];
                cv[3] = cc[j+3][i];
                // Compute
                float4 res = bv + cv;
                bb[j][i]   = res[0];
                bb[j+1][i] = res[1];
                bb[j+2][i] = res[2];
                bb[j+3][i] = res[3];
            }
#endif
            for (; j < 256; j++) {
                bb[j][i] = bb[j][i-1] + cc[j][i];
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
            int iterations = 5; float aa_scalar[128]; float aa_vector[128]; float bb_scalar[128]; float bb_vector[128]; float cc_scalar[128]; float cc_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(aa_scalar, n, &seed); memcpy(aa_vector, aa_scalar, sizeof(aa_scalar)); fill_f32(bb_scalar, n, &seed); memcpy(bb_vector, bb_scalar, sizeof(bb_scalar)); fill_f32(cc_scalar, n, &seed); memcpy(cc_vector, cc_scalar, sizeof(cc_scalar));
                s233(iterations, aa_scalar, bb_scalar, cc_scalar); vectorized_s233(iterations, aa_vector, bb_vector, cc_vector);
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
} for (int i = 0; i < n; ++i) {
    if (fabsf((cc_scalar[i]) - (cc_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter cc on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
