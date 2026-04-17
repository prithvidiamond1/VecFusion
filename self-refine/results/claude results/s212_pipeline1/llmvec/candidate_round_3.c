#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s212 (int n , float *a , float *b , float *c ,float * d ) {
    for (int i = 0; i < n -1; i ++) {
        a [ i ] *= c [ i ];
        b [ i ] += a [ i + 1] * d [ i ];
    }
}

        #include <stdint.h>

void vectorized_s212(int n, float *a, float *b, float *c, float *d) {
    typedef float float4 __attribute__((vector_size(16)));

    int limit = n - 1;
    int i = 0;

    /* Pass 1: b[i] += a[i+1] * d[i] using original a values */
    for (i = 0; i <= limit - 4; i += 4) {
        float4 ai1, di, bi;
        __builtin_memcpy(&ai1, &a[i+1], sizeof(float4));
        __builtin_memcpy(&di,  &d[i],   sizeof(float4));
        __builtin_memcpy(&bi,  &b[i],   sizeof(float4));
        bi = bi + ai1 * di;
        __builtin_memcpy(&b[i], &bi, sizeof(float4));
    }
    for (; i < limit; i++) {
        b[i] += a[i+1] * d[i];
    }

    /* Pass 2: a[i] *= c[i] */
    for (i = 0; i <= limit - 4; i += 4) {
        float4 ai, ci;
        __builtin_memcpy(&ai, &a[i], sizeof(float4));
        __builtin_memcpy(&ci, &c[i], sizeof(float4));
        ai = ai * ci;
        __builtin_memcpy(&a[i], &ai, sizeof(float4));
    }
    for (; i < limit; i++) {
        a[i] *= c[i];
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
            int n = n; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, n, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, n, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, n, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar));
                s212(n, a_scalar, b_scalar, c_scalar, d_scalar); vectorized_s212(n, a_vector, b_vector, c_vector, d_vector);
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
