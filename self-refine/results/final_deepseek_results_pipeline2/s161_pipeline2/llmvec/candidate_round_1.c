#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s161(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        // Store original a and c values before modifications
        float* a_orig = (float*)__builtin_alloca(LEN_1D * sizeof(float));
        float* c_orig = (float*)__builtin_alloca(LEN_1D * sizeof(float));
        __builtin_memcpy(a_orig, a, LEN_1D * sizeof(float));
        __builtin_memcpy(c_orig, c, LEN_1D * sizeof(float));

        // First: compute all a[i] updates using original c[i]
        for (int i = 0; i < LEN_1D-1; ++i) {
            if (b[i] >= (float)0.) {
                a[i] = c_orig[i] + d[i] * e[i];
            }
        }

        // Second: compute all c[i+1] updates using updated a[i] values
        for (int i = 0; i < LEN_1D-1; ++i) {
            if (b[i] < (float)0.) {
                // Use a[i] which may have been updated in first loop
                c[i+1] = a[i] + d[i] * d[i];
            }
        }
    }
}

        #include <stddef.h>

void vectorized_s161(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    int loop_bound = iterations / 2;

    for (int nl = 0; nl < loop_bound; nl++) {
        // Allocate temporary arrays
        float* a_orig = (float*)__builtin_alloca(LEN_1D * sizeof(float));
        float* c_orig = (float*)__builtin_alloca(LEN_1D * sizeof(float));

        // Copy original values
        for (int i = 0; i < LEN_1D; i++) {
            a_orig[i] = a[i];
            c_orig[i] = c[i];
        }

        // First loop: compute a[i] updates
        int i = 0;
        for (; i <= LEN_1D - 2; i++) {
            if (b[i] >= 0.0f) {
                a[i] = c_orig[i] + d[i] * e[i];
            }
        }

        // Second loop: compute c[i+1] updates
        for (i = 0; i <= LEN_1D - 2; i++) {
            if (b[i] < 0.0f) {
                c[i + 1] = a[i] + d[i] * d[i];
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
            const int arr_len = 128;
            uint32_t seed = 7u;
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128]; float d_scalar[128]; float d_vector[128]; float e_scalar[128]; float e_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar)); fill_f32(d_scalar, arr_len, &seed); memcpy(d_vector, d_scalar, sizeof(d_scalar)); fill_f32(e_scalar, arr_len, &seed); memcpy(e_vector, e_scalar, sizeof(e_scalar));
                s161(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar, e_scalar); vectorized_s161(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector, e_vector);
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
