#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s1244(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        // First compute all new a[i] values into a temporary array
        for (int i = 0; i < LEN_1D; i++) {
            a[i] = b[i] + c[i] * c[i] + b[i] * b[i] + c[i];
        }

        // Then compute d[i] using consecutive a[i] values
        for (int i = 0; i < LEN_1D-1; i++) {
            d[i] = a[i] + a[i+1];
        }
    }
}

        #include <stdint.h>

void vectorized_s1244(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    typedef float v4f __attribute__((vector_size(16)));
    const int VLEN = 4;

    for (int nl = 0; nl < iterations; nl++) {
        // Vectorized first loop: a[i] = b[i] + c[i]*c[i] + b[i]*b[i] + c[i]
        int i = 0;
        for (; i <= LEN_1D - VLEN; i += VLEN) {
            v4f bv, cv, av;
            __builtin_memcpy(&bv, b + i, sizeof(v4f));
            __builtin_memcpy(&cv, c + i, sizeof(v4f));
            av = bv + cv * cv + bv * bv + cv;
            __builtin_memcpy(a + i, &av, sizeof(v4f));
        }
        for (; i < LEN_1D; i++) {
            a[i] = b[i] + c[i] * c[i] + b[i] * b[i] + c[i];
        }

        // Vectorized second loop: d[i] = a[i] + a[i+1]
        i = 0;
        for (; i <= LEN_1D - 1 - VLEN; i += VLEN) {
            v4f a0, a1, dv;
            __builtin_memcpy(&a0, a + i,     sizeof(v4f));
            __builtin_memcpy(&a1, a + i + 1, sizeof(v4f));
            dv = a0 + a1;
            __builtin_memcpy(d + i, &dv, sizeof(v4f));
        }
        for (; i < LEN_1D - 1; i++) {
            d[i] = a[i] + a[i + 1];
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
                s1244(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar); vectorized_s1244(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector);
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
