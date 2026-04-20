#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s261(int iterations,int LEN_1D, float* a,float* b,float* c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Loop distribution: separate c update from a update
        // First compute all new c values into a temporary array
        float c_new[LEN_1D];
        for (int i = 1; i < LEN_1D; ++i) {
            c_new[i] = c[i] * d[i];
        }

        // Then compute a values using original c values
        for (int i = 1; i < LEN_1D; ++i) {
            float t = a[i] + b[i];
            a[i] = t + c[i-1];
        }

        // Copy back c values
        for (int i = 1; i < LEN_1D; ++i) {
            c[i] = c_new[i];
        }
    }
}

        #include <stddef.h>

void vectorized_s261(int iterations, int LEN_1D, float* a, float* b, float* c, float* d) {
    // Early exit for invalid inputs
    if (LEN_1D <= 1 || iterations <= 0) return;

    // Allocate temporary array for c_new
    float* c_new = (float*)__builtin_alloca(sizeof(float) * LEN_1D);

    for (int nl = 0; nl < iterations; nl++) {
        // Vectorize c_new computation
        int i = 1;
        // Process in chunks of 4
        for (; i + 3 < LEN_1D; i += 4) {
            c_new[i] = c[i] * d[i];
            c_new[i+1] = c[i+1] * d[i+1];
            c_new[i+2] = c[i+2] * d[i+2];
            c_new[i+3] = c[i+3] * d[i+3];
        }
        // Scalar tail
        for (; i < LEN_1D; i++) {
            c_new[i] = c[i] * d[i];
        }

        // Vectorize a update computation
        i = 1;
        // Process in chunks of 4
        for (; i + 3 < LEN_1D; i += 4) {
            float t0 = a[i] + b[i];
            float t1 = a[i+1] + b[i+1];
            float t2 = a[i+2] + b[i+2];
            float t3 = a[i+3] + b[i+3];

            a[i] = t0 + c[i-1];
            a[i+1] = t1 + c[i];
            a[i+2] = t2 + c[i+1];
            a[i+3] = t3 + c[i+2];
        }
        // Scalar tail
        for (; i < LEN_1D; i++) {
            float t = a[i] + b[i];
            a[i] = t + c[i-1];
        }

        // Vectorize c copy back
        i = 1;
        // Process in chunks of 4
        for (; i + 3 < LEN_1D; i += 4) {
            c[i] = c_new[i];
            c[i+1] = c_new[i+1];
            c[i+2] = c_new[i+2];
            c[i+3] = c_new[i+3];
        }
        // Scalar tail
        for (; i < LEN_1D; i++) {
            c[i] = c_new[i];
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
