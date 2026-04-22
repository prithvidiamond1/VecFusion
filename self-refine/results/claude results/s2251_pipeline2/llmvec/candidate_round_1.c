#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s2251(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations; nl++) {
        // i=0: s=0.0 (initial value)
        a[0] = 0.0f * e[0];
        float s = b[0] + c[0];
        b[0] = a[0] + d[0];

        // For i>=1: a[i] = s_prev * e[i], where s_prev = b[i-1]_orig + c[i-1]
        // Note: b[i-1] is already updated at this point, so we need original b[i-1]
        // s carries the value b[i-1]_orig + c[i-1] from previous iteration
        for (int i = 1; i < LEN_1D; i++) {
            a[i] = s * e[i];
            s = b[i] + c[i];
            b[i] = a[i] + d[i];
        }
    }
}

        #include <stdint.h>

void vectorized_s2251(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations; nl++) {
        // i=0: s=0.0 (initial value)
        a[0] = 0.0f * e[0];
        float s = b[0] + c[0];
        b[0] = a[0] + d[0];

        // The loop has a loop-carried dependency through s:
        //   s_{i} = b[i] + c[i]  (using original b[i])
        //   a[i]  = s_{i-1} * e[i]
        //   b[i]  = a[i] + d[i]
        //
        // Because s depends on the original b[i] (before update),
        // and a[i] depends on s from the previous iteration,
        // this is a serial recurrence. We can't fully vectorize the
        // dependency chain, but we can unroll to help the compiler
        // pipeline and reduce loop overhead.

        int i = 1;
        // Unroll by 4
        for (; i <= LEN_1D - 4; i += 4) {
            float s0 = s;
            float a0 = s0 * e[i];
            float s1 = b[i] + c[i];
            b[i] = a0 + d[i];
            a[i] = a0;

            float a1 = s1 * e[i+1];
            float s2 = b[i+1] + c[i+1];
            b[i+1] = a1 + d[i+1];
            a[i+1] = a1;

            float a2 = s2 * e[i+2];
            float s3 = b[i+2] + c[i+2];
            b[i+2] = a2 + d[i+2];
            a[i+2] = a2;

            float a3 = s3 * e[i+3];
            float s4 = b[i+3] + c[i+3];
            b[i+3] = a3 + d[i+3];
            a[i+3] = a3;

            s = s4;
        }

        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            a[i] = s * e[i];
            s = b[i] + c[i];
            b[i] = a[i] + d[i];
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
            int iterations = 5; int LEN_1D = n; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128]; float e_scalar[128]; float e_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, n, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, n, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, n, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar)); fill_f32(e_scalar, n, &seed); memcpy(e_vector, e_scalar, sizeof(e_scalar));
                s2251(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar, e_scalar); vectorized_s2251(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector, e_vector);
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
} for (int i = 0; i < n; ++i) {
    if (fabsf((e_scalar[i]) - (e_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter e on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
