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

        #include <stddef.h>

typedef float float32 __attribute__((ext_vector_type(4)));

void vectorized_s212(int n, float *a, float *b, float *c, float *d) {
    int i = 0;
    // Vectorize with 4-wide SIMD
    for (; i <= n - 5; i += 4) {
        float32 va = *(float32 *)&a[i];
        float32 vc = *(float32 *)&c[i];
        float32 vb = *(float32 *)&b[i];
        float32 vd = *(float32 *)&d[i];
        float32 va_next = *(float32 *)&a[i + 1];

        // a[i] *= c[i]
        va = va * vc;
        *(float32 *)&a[i] = va;

        // b[i] += a[i+1] * d[i]
        vb = vb + va_next * vd;
        *(float32 *)&b[i] = vb;
    }
    // Scalar cleanup for remaining iterations
    for (; i < n - 1; i++) {
        a[i] *= c[i];
        b[i] += a[i + 1] * d[i];
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
            int n = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, arr_len, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar));
                s212(n, a_scalar, b_scalar, c_scalar, d_scalar); vectorized_s212(n, a_vector, b_vector, c_vector, d_vector);
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
