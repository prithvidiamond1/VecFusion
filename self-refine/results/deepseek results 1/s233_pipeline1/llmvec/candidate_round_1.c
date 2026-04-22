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
            // but unroll manually
            int j = 1;
            for (; j + 3 < 256; j += 4) {
                aa[j][i]   = aa[j-1][i]   + cc[j][i];
                aa[j+1][i] = aa[j][i]     + cc[j+1][i];
                aa[j+2][i] = aa[j+1][i]   + cc[j+2][i];
                aa[j+3][i] = aa[j+2][i]   + cc[j+3][i];
            }
            for (; j < 256; j++) {
                aa[j][i] = aa[j-1][i] + cc[j][i];
            }

            // Second loop: bb[j][i] = bb[j][i-1] + cc[j][i]
            // No dependency on j, so we can vectorize over j
            j = 1;
#if defined(__clang__) || defined(__GNUC__)
            typedef float float4 __attribute__((vector_size(16)));
            // Process 16 floats at a time (4 vectors of 4)
            for (; j + 15 < 256; j += 16) {
                float4 cc0, cc1, cc2, cc3;
                float4 bb0, bb1, bb2, bb3;

                // Load cc[j..j+15][i]
                cc0 = (float4){cc[j][i],   cc[j+1][i],  cc[j+2][i],  cc[j+3][i]};
                cc1 = (float4){cc[j+4][i], cc[j+5][i],  cc[j+6][i],  cc[j+7][i]};
                cc2 = (float4){cc[j+8][i], cc[j+9][i],  cc[j+10][i], cc[j+11][i]};
                cc3 = (float4){cc[j+12][i],cc[j+13][i], cc[j+14][i], cc[j+15][i]};

                // Load bb[j..j+15][i-1]
                bb0 = (float4){bb[j][i-1],   bb[j+1][i-1],  bb[j+2][i-1],  bb[j+3][i-1]};
                bb1 = (float4){bb[j+4][i-1], bb[j+5][i-1],  bb[j+6][i-1],  bb[j+7][i-1]};
                bb2 = (float4){bb[j+8][i-1], bb[j+9][i-1],  bb[j+10][i-1], bb[j+11][i-1]};
                bb3 = (float4){bb[j+12][i-1],bb[j+13][i-1], bb[j+14][i-1], bb[j+15][i-1]};

                bb0 = bb0 + cc0;
                bb1 = bb1 + cc1;
                bb2 = bb2 + cc2;
                bb3 = bb3 + cc3;

                bb[j][i]    = bb0[0]; bb[j+1][i]  = bb0[1]; bb[j+2][i]  = bb0[2]; bb[j+3][i]  = bb0[3];
                bb[j+4][i]  = bb1[0]; bb[j+5][i]  = bb1[1]; bb[j+6][i]  = bb1[2]; bb[j+7][i]  = bb1[3];
                bb[j+8][i]  = bb2[0]; bb[j+9][i]  = bb2[1]; bb[j+10][i] = bb2[2]; bb[j+11][i] = bb2[3];
                bb[j+12][i] = bb3[0]; bb[j+13][i] = bb3[1]; bb[j+14][i] = bb3[2]; bb[j+15][i] = bb3[3];
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
            const int arr_len = 128;
            uint32_t seed = 7u;
            int iterations = 5; float aa_scalar[128]; float aa_vector[128]; float bb_scalar[128]; float bb_vector[128]; float cc_scalar[128]; float cc_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(aa_scalar, arr_len, &seed); memcpy(aa_vector, aa_scalar, sizeof(aa_scalar)); fill_f32(bb_scalar, arr_len, &seed); memcpy(bb_vector, bb_scalar, sizeof(bb_scalar)); fill_f32(cc_scalar, arr_len, &seed); memcpy(cc_vector, cc_scalar, sizeof(cc_scalar));
                s233(iterations, aa_scalar, bb_scalar, cc_scalar); vectorized_s233(iterations, aa_vector, bb_vector, cc_vector);
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
