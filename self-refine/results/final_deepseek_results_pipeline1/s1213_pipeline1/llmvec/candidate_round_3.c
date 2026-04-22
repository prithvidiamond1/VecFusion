#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s1213(int iterations, int LEN_1D, float* a,float* b,float* c,float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        for (int i = 1; i < LEN_1D-1; i++) {
            a[i] = b[i-1]+c[i];
            b[i] = a[i+1]*d[i];
        }
    }
}

        #include <stddef.h>

typedef float float32x4_t __attribute__((__vector_size__(16)));

void vectorized_s1213(int iterations, int LEN_1D, float* a, float* b, float* c, float* d) {
    if (LEN_1D < 3) return;

    for (int nl = 0; nl < iterations; nl++) {
        // Process first element separately to handle the dependency
        int i = 1;
        if (i < LEN_1D - 1) {
            a[i] = b[i-1] + c[i];
            b[i] = a[i+1] * d[i];
            i++;
        }

        // Vectorize remaining elements: process a and b in separate passes
        // First pass: compute all a values using original b values
        for (; i <= LEN_1D - 5; i += 4) {
            float32x4_t b_orig = *(float32x4_t*)(b + i - 1);
            float32x4_t c_vec = *(float32x4_t*)(c + i);
            float32x4_t a_new = b_orig + c_vec;
            *(float32x4_t*)(a + i) = a_new;
        }

        // Second pass: compute all b values using updated a values
        for (int j = 1; j < i; j += 4) {
            float32x4_t a_next = *(float32x4_t*)(a + j + 1);
            float32x4_t d_vec = *(float32x4_t*)(d + j);
            float32x4_t b_new = a_next * d_vec;
            *(float32x4_t*)(b + j) = b_new;
        }

        // Scalar cleanup for remaining elements
        for (; i < LEN_1D - 1; i++) {
            a[i] = b[i-1] + c[i];
            b[i] = a[i+1] * d[i];
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
                s1213(iterations, LEN_1D, a_scalar, b_scalar, c_scalar, d_scalar); vectorized_s1213(iterations, LEN_1D, a_vector, b_vector, c_vector, d_vector);
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
