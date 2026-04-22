#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s232(int iterations, float aa[256][256], float bb[256][256])
{
    for (int nl = 0; nl < 100*(iterations/(256)); nl++) {
        for (int j = 1; j < 256; j++) {
            for (int i = 1; i <= j; i++) {
                aa[j][i] = aa[j][i-1]*aa[j][i-1]+bb[j][i];
            }
        }
    }
}

        #include <stdint.h>

void vectorized_s232(int iterations, float aa[256][256], float bb[256][256])
{
    for (int nl = 0; nl < 100*(iterations/(256)); nl++) {
        for (int j = 1; j < 256; j++) {
            // The inner loop has a loop-carried dependency:
            // aa[j][i] = aa[j][i-1]*aa[j][i-1] + bb[j][i]
            // Each iteration depends on the previous result, so we cannot
            // vectorize across i. We unroll manually to help the compiler.
            int i = 1;
            // Unroll by 4, but note: due to the dependency chain, each
            // iteration still depends on the previous. We unroll for
            // instruction-level pipelining benefit.
            int limit = j - 3;
            for (; i <= limit; i += 4) {
                float v0 = aa[j][i-1];
                v0 = v0 * v0 + bb[j][i];
                aa[j][i] = v0;

                float v1 = v0;
                v1 = v1 * v1 + bb[j][i+1];
                aa[j][i+1] = v1;

                float v2 = v1;
                v2 = v2 * v2 + bb[j][i+2];
                aa[j][i+2] = v2;

                float v3 = v2;
                v3 = v3 * v3 + bb[j][i+3];
                aa[j][i+3] = v3;
            }
            // Scalar cleanup
            for (; i <= j; i++) {
                aa[j][i] = aa[j][i-1] * aa[j][i-1] + bb[j][i];
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
                s232(iterations, aa_scalar, bb_scalar); vectorized_s232(iterations, aa_vector, bb_vector);
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
