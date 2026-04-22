#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s211(int iterations, int LEN_1D, float*a, float*b,float*c,float*d,float*e)
{
    for (int nl = 0; nl < iterations; nl++) {
        // First compute all b[i] updates using original b[i+1] values
        for (int i = 1; i < LEN_1D-1; i++) {
            float temp_b_next = b[i+1];
            b[i] = temp_b_next - e[i] * d[i];
        }

        // Then compute all a[i] using updated b[i-1] values
        for (int i = 1; i < LEN_1D-1; i++) {
            a[i] = b[i-1] + c[i] * d[i];
        }
    }
}

        #include <string.h>

void vectorized_s211(int iterations, int LEN_1D, float*a, float*b, float*c, float*d, float*e)
{
    for (int nl = 0; nl < iterations; nl++) {
        // First loop: b[i] = b[i+1] - e[i] * d[i], for i = 1 .. LEN_1D-2
        // b[i+1] are the original values read before any writes in this loop.
        // Since we write b[i] and read b[i+1], and i goes forward, b[i+1] hasn't
        // been written yet when we read it. So we can vectorize directly.
        int i = 1;
        int len = LEN_1D - 2; // number of iterations: i from 1 to LEN_1D-2 inclusive

        // Vector width: 8 floats
        #define VW 8
        typedef float float8 __attribute__((vector_size(VW * sizeof(float))));

        int vec_end = 1 + (len / VW) * VW;
        for (; i < vec_end; i += VW) {
            float8 b_next, ei, di, res;
            __builtin_memcpy(&b_next, &b[i+1], sizeof(float8));
            __builtin_memcpy(&ei, &e[i], sizeof(float8));
            __builtin_memcpy(&di, &d[i], sizeof(float8));
            res = b_next - ei * di;
            __builtin_memcpy(&b[i], &res, sizeof(float8));
        }
        // Scalar tail
        for (; i < LEN_1D - 1; i++) {
            b[i] = b[i+1] - e[i] * d[i];
        }

        // Second loop: a[i] = b[i-1] + c[i] * d[i], for i = 1 .. LEN_1D-2
        i = 1;
        vec_end = 1 + (len / VW) * VW;
        for (; i < vec_end; i += VW) {
            float8 b_prev, ci, di, res;
            __builtin_memcpy(&b_prev, &b[i-1], sizeof(float8));
            __builtin_memcpy(&ci, &c[i], sizeof(float8));
            __builtin_memcpy(&di, &d[i], sizeof(float8));
            res = b_prev + ci * di;
            __builtin_memcpy(&a[i], &res, sizeof(float8));
        }
        // Scalar tail
        for (; i < LEN_1D - 1; i++) {
            a[i] = b[i-1] + c[i] * d[i];
        }

        #undef VW
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128]; float e_scalar[128]; float e_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, arr_len, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar)); fill_f32(e_scalar, arr_len, &seed); memcpy(e_vector, e_scalar, sizeof(e_scalar));
                s211(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar, e_scalar); vectorized_s211(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector, e_vector);
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
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((e_scalar[i]) - (e_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter e on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
