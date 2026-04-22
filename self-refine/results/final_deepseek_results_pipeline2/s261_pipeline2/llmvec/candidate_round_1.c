#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s261(int iterations,int LEN_1D, float* a,float* b,float* c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Reorder instructions to break the loop-carried dependency on c[i]
        // First compute all new c[i] values, then compute a[i] using updated c[i-1]
        for (int i = 1; i < LEN_1D; ++i) {
            c[i] = c[i] * d[i];
        }
        for (int i = 1; i < LEN_1D; ++i) {
            a[i] = (a[i] + b[i]) + c[i-1];
        }
    }
}

        #include <stddef.h>

void vectorized_s261(int iterations, int LEN_1D, float* a, float* b, float* c, float* d) {
    if (LEN_1D <= 1 || iterations <= 0) return;

    for (int nl = 0; nl < iterations; nl++) {
        // Vectorize c[i] = c[i] * d[i] for i >= 1
        int i = 1;
        int end = LEN_1D;

        // Main vectorized loop for c update
        for (; i + 3 < end; i += 4) {
            c[i] = c[i] * d[i];
            c[i+1] = c[i+1] * d[i+1];
            c[i+2] = c[i+2] * d[i+2];
            c[i+3] = c[i+3] * d[i+3];
        }
        // Scalar tail for c update
        for (; i < end; i++) {
            c[i] = c[i] * d[i];
        }

        // Vectorize a[i] = (a[i] + b[i]) + c[i-1] for i >= 1
        i = 1;
        // Main vectorized loop for a update
        for (; i + 3 < end; i += 4) {
            a[i] = (a[i] + b[i]) + c[i-1];
            a[i+1] = (a[i+1] + b[i+1]) + c[i];
            a[i+2] = (a[i+2] + b[i+2]) + c[i+1];
            a[i+3] = (a[i+3] + b[i+3]) + c[i+2];
        }
        // Scalar tail for a update
        for (; i < end; i++) {
            a[i] = (a[i] + b[i]) + c[i-1];
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
                s261(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar); vectorized_s261(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector);
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
