#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s256(int iterations, float* a,float* d, float aa[256][256],float bb[256][256])
{
    for (int nl = 0; nl < 10*(iterations/256); nl++) {
        for (int i = 0; i < 256; i++) {
            for (int j = 1; j < 256; j++) {
                a[j] = (float)1.0 - a[j - 1];
                aa[j][i] = a[j] + bb[j][i]*d[j];
            }
        }
    }
}

        #include <stdint.h>

void vectorized_s256(int iterations, float* a, float* d, float aa[256][256], float bb[256][256])
{
    for (int nl = 0; nl < 10*(iterations/256); nl++) {
        for (int i = 0; i < 256; i++) {
            // j=1: a[1] = 1.0f - a[0]
            // j=2: a[2] = 1.0f - a[1]
            // Each iteration depends on previous a[j-1], so this is a recurrence.
            // We cannot vectorize across j due to the dependency a[j] = 1.0f - a[j-1].
            // However, we can observe the pattern:
            // a[j] = 1.0f - a[j-1]
            // a[j+1] = 1.0f - a[j] = 1.0f - (1.0f - a[j-1]) = a[j-1]
            // So the sequence alternates: a[j] and a[j-1] alternate between two values.
            // Let v0 = a[0], then:
            //   a[1] = 1.0f - v0
            //   a[2] = v0
            //   a[3] = 1.0f - v0
            //   a[4] = v0
            // For even j: a[j] = v0 (j >= 2)
            // For odd j:  a[j] = 1.0f - v0
            // We can precompute a[j] for all j, then vectorize the aa computation.

            float v0 = a[0];
            float v1 = 1.0f - v0;

            // Fill a[j] for j=1..255
            // odd j: v1, even j: v0
            for (int j = 1; j < 256; j++) {
                a[j] = (j & 1) ? v1 : v0;
            }

            // Now vectorize the aa computation in chunks of 8
            int j = 1;
            for (; j <= 252; j += 4) {
                float aj0 = a[j+0];
                float aj1 = a[j+1];
                float aj2 = a[j+2];
                float aj3 = a[j+3];

                float dj0 = d[j+0];
                float dj1 = d[j+1];
                float dj2 = d[j+2];
                float dj3 = d[j+3];

                float bbj0 = bb[j+0][i];
                float bbj1 = bb[j+1][i];
                float bbj2 = bb[j+2][i];
                float bbj3 = bb[j+3][i];

                aa[j+0][i] = aj0 + bbj0 * dj0;
                aa[j+1][i] = aj1 + bbj1 * dj1;
                aa[j+2][i] = aj2 + bbj2 * dj2;
                aa[j+3][i] = aj3 + bbj3 * dj3;
            }
            // scalar tail
            for (; j < 256; j++) {
                aa[j][i] = a[j] + bb[j][i] * d[j];
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
            int iterations = 5; float a_scalar[128]; float a_vector[128]; float d_scalar[128]; float d_vector[128]; float aa_scalar[128]; float aa_vector[128]; float bb_scalar[128]; float bb_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(d_scalar, n, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar)); fill_f32(aa_scalar, n, &seed); memcpy(aa_vector, aa_scalar, sizeof(aa_scalar)); fill_f32(bb_scalar, n, &seed); memcpy(bb_vector, bb_scalar, sizeof(bb_scalar));
                s256(iterations, a_scalar, d_scalar, aa_scalar, bb_scalar); vectorized_s256(iterations, a_vector, d_vector, aa_vector, bb_vector);
                for (int i = 0; i < n; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((d_scalar[i]) - (d_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter d on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
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
