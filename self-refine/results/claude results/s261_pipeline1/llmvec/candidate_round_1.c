#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s261(int iterations,int LEN_1D, float* a,float* b,float* c, float* d)
{
    float t;
    for (int nl = 0; nl < iterations; nl++) {
        for (int i = 1; i < LEN_1D; ++i) {
            t = a[i] + b[i];
            a[i] = t + c[i-1];
            t = c[i] * d[i];
            c[i] = t;
        }
    }
}

        #include <stddef.h>

void vectorized_s261(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        // c has a loop-carried dependency: c[i] = c[i]*d[i], and a[i] uses c[i-1] (updated)
        // We must compute c serially, but can unroll by 4 with scalar steps
        int i = 1;
        // Unroll by 4 - but c dependency is serial, so we compute c first then a
        // Actually interleave: compute c[i-1] first (already done), use it for a[i], then update c[i]
        for (; i <= LEN_1D - 4 - 1; i += 4) {
            float t0, t1, t2, t3;

            // a[i] = a[i] + b[i] + c[i-1] (c[i-1] already updated)
            t0 = a[i+0] + b[i+0] + c[i-1+0];
            // c[i] = c[i] * d[i]
            float ci0 = c[i+0] * d[i+0];

            t1 = a[i+1] + b[i+1] + ci0;
            float ci1 = c[i+1] * d[i+1];

            t2 = a[i+2] + b[i+2] + ci1;
            float ci2 = c[i+2] * d[i+2];

            t3 = a[i+3] + b[i+3] + ci2;
            float ci3 = c[i+3] * d[i+3];

            a[i+0] = t0;
            a[i+1] = t1;
            a[i+2] = t2;
            a[i+3] = t3;

            c[i+0] = ci0;
            c[i+1] = ci1;
            c[i+2] = ci2;
            c[i+3] = ci3;
        }
        // scalar tail
        for (; i < LEN_1D; ++i) {
            float t = a[i] + b[i];
            a[i] = t + c[i-1];
            t = c[i] * d[i];
            c[i] = t;
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
            int iterations = 5; int LEN_1D = n; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, n, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, n, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, n, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar));
                s261(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar); vectorized_s261(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector);
                for (int i = 0; i < n; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((b_scalar[i]) - (b_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter b on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((c_scalar[i]) - (c_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter c on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((d_scalar[i]) - (d_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter d on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
