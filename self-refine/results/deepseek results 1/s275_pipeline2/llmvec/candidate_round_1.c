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
        // Precompute condition mask for all columns
        int cond[256];
        for (int i = 0; i < 256; i++) {
            cond[i] = aa[0][i] > 0.f;
        }

        // Process each row sequentially (dependency in j direction)
        for (int j = 1; j < 256; j++) {
            // Vector-friendly loop across columns
            for (int i = 0; i < 256; i++) {
                if (cond[i]) {
                    aa[j][i] = aa[j-1][i] + bb[j][i] * cc[j][i];
                }
            }
        }
    }
}

        #include <string.h>

void vectorized_s275(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    int loop_count = 10 * (iterations / 256);

    for (int nl = 0; nl < loop_count; nl++) {
        // Precompute condition mask for all columns
        int cond[256];
        for (int i = 0; i < 256; i++) {
            cond[i] = aa[0][i] > 0.f;
        }

        // Process each row sequentially (dependency in j direction)
        for (int j = 1; j < 256; j++) {
            // Unrolled vector-friendly loop across columns (4-wide)
            int i = 0;
            for (; i <= 252; i += 4) {
                float a0 = aa[j-1][i+0];
                float a1 = aa[j-1][i+1];
                float a2 = aa[j-1][i+2];
                float a3 = aa[j-1][i+3];

                float b0 = bb[j][i+0];
                float b1 = bb[j][i+1];
                float b2 = bb[j][i+2];
                float b3 = bb[j][i+3];

                float c0 = cc[j][i+0];
                float c1 = cc[j][i+1];
                float c2 = cc[j][i+2];
                float c3 = cc[j][i+3];

                float r0 = a0 + b0 * c0;
                float r1 = a1 + b1 * c1;
                float r2 = a2 + b2 * c2;
                float r3 = a3 + b3 * c3;

                if (cond[i+0]) aa[j][i+0] = r0;
                if (cond[i+1]) aa[j][i+1] = r1;
                if (cond[i+2]) aa[j][i+2] = r2;
                if (cond[i+3]) aa[j][i+3] = r3;
            }
            // Scalar cleanup tail
            for (; i < 256; i++) {
                if (cond[i]) {
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
