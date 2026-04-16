#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s242(int iterations, int LEN_1D, float s1, float s2, float* a, float* b, float* c, float* d)
{
    for (int nl = 0; nl < iterations/5; nl++) {
        float prev = a[0];
        for (int i = 1; i < LEN_1D; ++i) {
            float curr = prev + s1 + s2 + b[i] + c[i] + d[i];
            a[i] = curr;
            prev = curr;
        }
    }
}

        void vectorized_s242(int iterations, int LEN_1D, float s1, float s2, float* a, float* b, float* c, float* d)
{
    for (int nl = 0; nl < iterations/5; nl++) {
        float prev = a[0];
        float s12 = s1 + s2;
        // This loop has a loop-carried dependency through prev,
        // so we cannot fully vectorize the inner loop in the traditional sense.
        // However, we can unroll it to help the compiler and improve ILP.
        int i = 1;
        // Unroll by 4 - note: each iteration depends on previous, so we compute sequentially
        // but unrolling helps with instruction scheduling
        for (; i <= LEN_1D - 4; i += 4) {
            float c0 = prev   + s12 + b[i]   + c[i]   + d[i];
            float c1 = c0     + s12 + b[i+1] + c[i+1] + d[i+1];
            float c2 = c1     + s12 + b[i+2] + c[i+2] + d[i+2];
            float c3 = c2     + s12 + b[i+3] + c[i+3] + d[i+3];
            a[i]   = c0;
            a[i+1] = c1;
            a[i+2] = c2;
            a[i+3] = c3;
            prev = c3;
        }
        // Scalar cleanup
        for (; i < LEN_1D; i++) {
            float curr = prev + s12 + b[i] + c[i] + d[i];
            a[i] = curr;
            prev = curr;
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
            int iterations = 5; int LEN_1D = n; float s1 = 7; float s2 = 7; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, n, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, n, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, n, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar));
                s242(iterations, LEN_1D, s1, s2, a_scalar, b_scalar, c_scalar, d_scalar); vectorized_s242(iterations, LEN_1D, s1, s2, a_vector, b_vector, c_vector, d_vector);
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
