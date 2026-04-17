#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s2111(int iterations,float aa[256][256])
{
    for (int nl = 0; nl < 100*(iterations/(256)); nl++) {
        for (int j = 1; j < 256; j++) {
            for (int i = 1; i < 256; i++) {
                aa[j][i] = (aa[j][i-1] + aa[j-1][i])/1.9;
            }
        }
    }
}

        #include <stdint.h>

void vectorized_s2111(int iterations, float aa[256][256])
{
    int limit = 100 * (iterations / 256);
    for (int nl = 0; nl < limit; nl++) {
        for (int j = 1; j < 256; j++) {
            // Each element depends on aa[j][i-1] (same row, previous col)
            // and aa[j-1][i] (previous row, same col).
            // The dependency on aa[j][i-1] means we cannot vectorize the inner
            // loop directly without breaking semantics. We process element by
            // element but unroll manually to hint the compiler.
            int i = 1;
            for (; i <= 252; i += 4) {
                aa[j][i]   = (aa[j][i-1]   + aa[j-1][i])   / 1.9f;
                aa[j][i+1] = (aa[j][i]     + aa[j-1][i+1]) / 1.9f;
                aa[j][i+2] = (aa[j][i+1]   + aa[j-1][i+2]) / 1.9f;
                aa[j][i+3] = (aa[j][i+2]   + aa[j-1][i+3]) / 1.9f;
            }
            for (; i < 256; i++) {
                aa[j][i] = (aa[j][i-1] + aa[j-1][i]) / 1.9f;
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
            int iterations = 5; float aa_scalar[128]; float aa_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(aa_scalar, n, &seed); memcpy(aa_vector, aa_scalar, sizeof(aa_scalar));
                s2111(iterations, aa_scalar); vectorized_s2111(iterations, aa_vector);
                for (int i = 0; i < n; ++i) {
    if (fabsf((aa_scalar[i]) - (aa_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter aa on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
