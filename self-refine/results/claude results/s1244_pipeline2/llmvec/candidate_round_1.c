#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s1244(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Save original a[] values before updating
        float a_prev = a[0];
        for (int i = 0; i < LEN_1D-1; i++) {
            float a_next = a[i+1];
            a[i] = b[i] + c[i] * c[i] + b[i]*b[i] + c[i];
            d[i] = a[i] + a_next;
        }
    }
}

        #include <stdint.h>

void vectorized_s1244(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        // We need to compute new a[i] = b[i] + c[i]*c[i] + b[i]*b[i] + c[i]
        // and d[i] = new_a[i] + a[i+1] (original a[i+1])
        // Since a[i+1] is read before a[i+1] is written (we write a[i] and read a[i+1]),
        // we can vectorize by saving a[i+1] values first or processing carefully.
        // The dependency: a[i] is written, a[i+1] is read (original). Since i+1 > i,
        // we read a[i+1] before we write it (we write a[i] in iteration i, a[i+1] in iteration i+1).
        // So we can vectorize safely if we read a[i+1] before writing a[i].

        int i = 0;
        int limit = LEN_1D - 1;

        // Process in chunks of 8
        int vec_limit = limit - (limit % 8);

        for (; i < vec_limit; i += 8) {
            // Load original a[i+1..i+8] before overwriting a[i..i+7]
            float a_next0 = a[i+1];
            float a_next1 = a[i+2];
            float a_next2 = a[i+3];
            float a_next3 = a[i+4];
            float a_next4 = a[i+5];
            float a_next5 = a[i+6];
            float a_next6 = a[i+7];
            float a_next7 = a[i+8];

            float b0 = b[i+0], b1 = b[i+1], b2 = b[i+2], b3 = b[i+3];
            float b4 = b[i+4], b5 = b[i+5], b6 = b[i+6], b7 = b[i+7];

            float c0 = c[i+0], c1 = c[i+1], c2 = c[i+2], c3 = c[i+3];
            float c4 = c[i+4], c5 = c[i+5], c6 = c[i+6], c7 = c[i+7];

            float na0 = b0 + c0*c0 + b0*b0 + c0;
            float na1 = b1 + c1*c1 + b1*b1 + c1;
            float na2 = b2 + c2*c2 + b2*b2 + c2;
            float na3 = b3 + c3*c3 + b3*b3 + c3;
            float na4 = b4 + c4*c4 + b4*b4 + c4;
            float na5 = b5 + c5*c5 + b5*b5 + c5;
            float na6 = b6 + c6*c6 + b6*b6 + c6;
            float na7 = b7 + c7*c7 + b7*b7 + c7;

            a[i+0] = na0; a[i+1] = na1; a[i+2] = na2; a[i+3] = na3;
            a[i+4] = na4; a[i+5] = na5; a[i+6] = na6; a[i+7] = na7;

            d[i+0] = na0 + a_next0;
            d[i+1] = na1 + a_next1;
            d[i+2] = na2 + a_next2;
            d[i+3] = na3 + a_next3;
            d[i+4] = na4 + a_next4;
            d[i+5] = na5 + a_next5;
            d[i+6] = na6 + a_next6;
            d[i+7] = na7 + a_next7;
        }

        // Scalar cleanup
        for (; i < limit; i++) {
            float a_next = a[i+1];
            a[i] = b[i] + c[i]*c[i] + b[i]*b[i] + c[i];
            d[i] = a[i] + a_next;
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
                s1244(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar); vectorized_s1244(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector);
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
