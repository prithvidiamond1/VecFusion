#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s275(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    int loop_count = 10 * (iterations / 256);

    for (int nl = 0; nl < loop_count; nl++) {
        // First, identify which columns need processing
        int process_cols[256];
        int col_count = 0;

        // Vector-friendly: process all columns, mask with condition
        for (int i = 0; i < 256; i++) {
            process_cols[i] = i;
            col_count += (aa[0][i] > (float)0.);
        }

        // Process columns in vector-friendly order: j outer, i inner
        for (int j = 1; j < 256; j++) {
            // Process all columns with conditional execution
            for (int i = 0; i < 256; i++) {
                // Use masking to avoid branching
                float condition = (aa[0][i] > (float)0.) ? 1.0f : 0.0f;
                float temp = aa[j-1][i] + bb[j][i] * cc[j][i];
                aa[j][i] = condition * temp + (1.0f - condition) * aa[j][i];
            }
        }
    }
}

        #include <string.h>

typedef float v8sf __attribute__((vector_size(32)));

void vectorized_s275(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    int loop_count = 10 * (iterations / 256);

    for (int nl = 0; nl < loop_count; nl++) {
        // Precompute condition mask for row 0
        float cond[256];
        for (int i = 0; i < 256; i++) {
            cond[i] = (aa[0][i] > 0.f) ? 1.0f : 0.0f;
        }

        for (int j = 1; j < 256; j++) {
            float *aaj   = aa[j];
            float *aajm1 = aa[j-1];
            float *bbj   = bb[j];
            float *ccj   = cc[j];

            int i = 0;
            for (; i <= 256 - 8; i += 8) {
                v8sf c    = *(v8sf*)(&cond[i]);
                v8sf one  = {1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f};
                v8sf omc  = one - c;
                v8sf prev = *(v8sf*)(&aajm1[i]);
                v8sf b    = *(v8sf*)(&bbj[i]);
                v8sf cc_v = *(v8sf*)(&ccj[i]);
                v8sf cur  = *(v8sf*)(&aaj[i]);
                v8sf temp = prev + b * cc_v;
                v8sf res  = c * temp + omc * cur;
                *(v8sf*)(&aaj[i]) = res;
            }
            // scalar tail
            for (; i < 256; i++) {
                float c    = cond[i];
                float temp = aajm1[i] + bbj[i] * ccj[i];
                aaj[i] = c * temp + (1.0f - c) * aaj[i];
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
