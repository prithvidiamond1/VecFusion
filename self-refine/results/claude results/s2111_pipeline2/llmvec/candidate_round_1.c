#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s2111(int iterations, float aa[256][256])
{
    for (int nl = 0; nl < 100*(iterations/(256)); nl++) {
        // Wavefront (diagonal) traversal to eliminate loop-carried dependencies
        // Elements on the same anti-diagonal d = i + j have no dependencies on each other
        for (int d = 2; d <= 510; d++) {
            int j_start = (d - 255 > 1) ? d - 255 : 1;
            int j_end   = (d - 1 < 255) ? d - 1 : 255;
            for (int j = j_start; j <= j_end; j++) {
                int i = d - j;
                aa[j][i] = (aa[j][i-1] + aa[j-1][i]) / 1.9f;
            }
        }
    }
}

        #include <stdint.h>

void vectorized_s2111(int iterations, float aa[256][256])
{
    for (int nl = 0; nl < 100*(iterations/(256)); nl++) {
        for (int d = 2; d <= 510; d++) {
            int j_start = (d - 255 > 1) ? d - 255 : 1;
            int j_end   = (d - 1 < 255) ? d - 1 : 255;
            int len = j_end - j_start + 1;

            // Unroll by 4 for vector-friendly access
            int j = j_start;
            int limit4 = j_start + (len & ~3);

            for (; j < limit4; j += 4) {
                int i0 = d - j;
                int i1 = d - (j+1);
                int i2 = d - (j+2);
                int i3 = d - (j+3);

                float v0 = (aa[j  ][i0-1] + aa[j-1  ][i0]) / 1.9f;
                float v1 = (aa[j+1][i1-1] + aa[j  ][i1]) / 1.9f;
                float v2 = (aa[j+2][i2-1] + aa[j+1][i2]) / 1.9f;
                float v3 = (aa[j+3][i3-1] + aa[j+2][i3]) / 1.9f;

                aa[j  ][i0] = v0;
                aa[j+1][i1] = v1;
                aa[j+2][i2] = v2;
                aa[j+3][i3] = v3;
            }

            // Scalar tail
            for (; j <= j_end; j++) {
                int i = d - j;
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
