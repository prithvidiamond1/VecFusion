#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s482(int iterations, int LEN_1D, float* a, float *b, float *c)
{
    for (int nl = 0; nl < iterations; nl++) {
        int break_index = LEN_1D;

        // Find break point first (unchanged values)
        for (int i = 0; i < LEN_1D; i++) {
            if (c[i] > b[i]) {
                break_index = i;
                break;
            }
        }

        // Vector-friendly loop without conditional break
        for (int i = 0; i < break_index; i++) {
            a[i] += b[i] * c[i];
        }

        // Handle the break index element separately
        if (break_index < LEN_1D) {
            a[break_index] += b[break_index] * c[break_index];
        }
    }
}

        #include <stddef.h>

void vectorized_s482(int iterations, int LEN_1D, float* a, float *b, float *c) {
    for (int nl = 0; nl < iterations; nl++) {
        int break_index = LEN_1D;

        // Find break point first (scalar search)
        for (int i = 0; i < LEN_1D; i++) {
            if (c[i] > b[i]) {
                break_index = i;
                break;
            }
        }

        // Vector-friendly main loop
        int i = 0;
        for (; i + 3 < break_index; i += 4) {
            a[i] += b[i] * c[i];
            a[i+1] += b[i+1] * c[i+1];
            a[i+2] += b[i+2] * c[i+2];
            a[i+3] += b[i+3] * c[i+3];
        }

        // Scalar tail
        for (; i < break_index; i++) {
            a[i] += b[i] * c[i];
        }

        // Handle the break index element separately
        if (break_index < LEN_1D) {
            a[break_index] += b[break_index] * c[break_index];
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar));
                s482(iterations, LEN_1D, a_scalar, b_scalar, c_scalar); vectorized_s482(iterations, LEN_1D, a_vector, b_vector, c_vector);
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
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
