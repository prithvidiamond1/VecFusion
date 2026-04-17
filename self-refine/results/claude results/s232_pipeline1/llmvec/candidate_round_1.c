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

        void vectorized_s232(int iterations, float aa[256][256], float bb[256][256])
{
    for (int nl = 0; nl < 100*(iterations/(256)); nl++) {
        for (int j = 1; j < 256; j++) {
            // The inner loop has a loop-carried dependency:
            // aa[j][i] = aa[j][i-1]*aa[j][i-1] + bb[j][i]
            // Each iteration depends on the previous, so true vectorization
            // is not possible. We unroll manually to help the compiler.
            int i = 1;
            // Unroll by 4, but note: due to the dependency chain,
            // each iteration truly depends on the previous result.
            // We unroll to reduce loop overhead while preserving semantics.
            for (; i + 3 <= j; i += 4) {
                float v0 = aa[j][i-1];
                float r0 = v0 * v0 + bb[j][i];
                aa[j][i] = r0;

                float r1 = r0 * r0 + bb[j][i+1];
                aa[j][i+1] = r1;

                float r2 = r1 * r1 + bb[j][i+2];
                aa[j][i+2] = r2;

                float r3 = r2 * r2 + bb[j][i+3];
                aa[j][i+3] = r3;
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
            const int n = 128;
            uint32_t seed = 7u;
            int iterations = 5; float aa_scalar[128]; float aa_vector[128]; float bb_scalar[128]; float bb_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(aa_scalar, n, &seed); memcpy(aa_vector, aa_scalar, sizeof(aa_scalar)); fill_f32(bb_scalar, n, &seed); memcpy(bb_vector, bb_scalar, sizeof(bb_scalar));
                s232(iterations, aa_scalar, bb_scalar); vectorized_s232(iterations, aa_vector, bb_vector);
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
