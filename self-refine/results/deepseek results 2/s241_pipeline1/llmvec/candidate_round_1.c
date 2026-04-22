#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s241(int iterations, int LEN_1D, float* a,float* b,float* c,float* d)
{
    for (int nl = 0; nl < 2*iterations; nl++) {
        for (int i = 0; i < LEN_1D-1; i++) {
            a[i] = b[i] * c[i  ] * d[i];
            b[i] = a[i] * a[i+1] * d[i];
        }
    }
}

        void vectorized_s241(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    for (int nl = 0; nl < 2 * iterations; nl++) {
        // The inner loop has a loop-carried dependency through a[i+1] in the b[i] update,
        // and b[i] is read in the a[i] update but written after. We must preserve scalar semantics.
        // a[i] = b[i] * c[i] * d[i]  (uses old b[i])
        // b[i] = a[i] * a[i+1] * d[i] (uses new a[i] and new a[i+1])
        // Since a[i+1] is computed in the next iteration, we can split into two passes:
        // Pass 1: compute all a[i] from old b[i], c[i], d[i]
        // Pass 2: compute all b[i] from new a[i], a[i+1], d[i]

        int i = 0;
        int limit = LEN_1D - 1;

        // Pass 1: a[i] = b[i] * c[i] * d[i] for i in [0, LEN_1D-2]
        // Vectorize with width 8
        int vec_limit = limit - (limit % 8);
        for (i = 0; i < vec_limit; i += 8) {
            a[i+0] = b[i+0] * c[i+0] * d[i+0];
            a[i+1] = b[i+1] * c[i+1] * d[i+1];
            a[i+2] = b[i+2] * c[i+2] * d[i+2];
            a[i+3] = b[i+3] * c[i+3] * d[i+3];
            a[i+4] = b[i+4] * c[i+4] * d[i+4];
            a[i+5] = b[i+5] * c[i+5] * d[i+5];
            a[i+6] = b[i+6] * c[i+6] * d[i+6];
            a[i+7] = b[i+7] * c[i+7] * d[i+7];
        }
        for (; i < limit; i++) {
            a[i] = b[i] * c[i] * d[i];
        }

        // Pass 2: b[i] = a[i] * a[i+1] * d[i] for i in [0, LEN_1D-2]
        vec_limit = limit - (limit % 8);
        for (i = 0; i < vec_limit; i += 8) {
            b[i+0] = a[i+0] * a[i+1] * d[i+0];
            b[i+1] = a[i+1] * a[i+2] * d[i+1];
            b[i+2] = a[i+2] * a[i+3] * d[i+2];
            b[i+3] = a[i+3] * a[i+4] * d[i+3];
            b[i+4] = a[i+4] * a[i+5] * d[i+4];
            b[i+5] = a[i+5] * a[i+6] * d[i+5];
            b[i+6] = a[i+6] * a[i+7] * d[i+6];
            b[i+7] = a[i+7] * a[i+8] * d[i+7];
        }
        for (; i < limit; i++) {
            b[i] = a[i] * a[i+1] * d[i];
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, arr_len, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar));
                s241(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar); vectorized_s241(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector);
                for (int i = 0; i < arr_len; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((b_scalar[i]) - (b_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter b on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((c_scalar[i]) - (c_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter c on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((d_scalar[i]) - (d_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter d on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
