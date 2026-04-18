#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s281(int iterations,int LEN_1D, float* a,float* b,float* c)
{
    float* a_original = (float*)malloc(LEN_1D * sizeof(float));

    for (int nl = 0; nl < iterations; nl++) {
        // Copy 'a' to temporary array to preserve original values
        for (int i = 0; i < LEN_1D; i++) {
            a_original[i] = a[i];
        }

        // Process all iterations using preserved 'a' values
        for (int i = 0; i < LEN_1D; i++) {
            float x = a_original[LEN_1D-i-1] + b[i] * c[i];
            a[i] = x - (float)1.0;
            b[i] = x;
        }
    }

    free(a_original);
}

        #include <stdlib.h>
#include <string.h>

void vectorized_s281(int iterations, int LEN_1D, float* a, float* b, float* c)
{
    float* a_original = (float*)malloc(LEN_1D * sizeof(float));

    for (int nl = 0; nl < iterations; nl++) {
        // Copy 'a' to temporary array to preserve original values
        memcpy(a_original, a, LEN_1D * sizeof(float));

        // Process all iterations using preserved 'a' values
        // Vectorize: x[i] = a_original[LEN_1D-i-1] + b[i] * c[i]
        // a[i] = x[i] - 1.0f
        // b[i] = x[i]
        // Note: a_original[LEN_1D-i-1] is a reverse-indexed read, no write dependency.

        int i = 0;
        int vec_len = LEN_1D - (LEN_1D % 8);

        for (; i < vec_len; i += 8) {
            float x0 = a_original[LEN_1D - i - 1] + b[i]   * c[i];
            float x1 = a_original[LEN_1D - i - 2] + b[i+1] * c[i+1];
            float x2 = a_original[LEN_1D - i - 3] + b[i+2] * c[i+2];
            float x3 = a_original[LEN_1D - i - 4] + b[i+3] * c[i+3];
            float x4 = a_original[LEN_1D - i - 5] + b[i+4] * c[i+4];
            float x5 = a_original[LEN_1D - i - 6] + b[i+5] * c[i+5];
            float x6 = a_original[LEN_1D - i - 7] + b[i+6] * c[i+6];
            float x7 = a_original[LEN_1D - i - 8] + b[i+7] * c[i+7];

            a[i]   = x0 - 1.0f;
            a[i+1] = x1 - 1.0f;
            a[i+2] = x2 - 1.0f;
            a[i+3] = x3 - 1.0f;
            a[i+4] = x4 - 1.0f;
            a[i+5] = x5 - 1.0f;
            a[i+6] = x6 - 1.0f;
            a[i+7] = x7 - 1.0f;

            b[i]   = x0;
            b[i+1] = x1;
            b[i+2] = x2;
            b[i+3] = x3;
            b[i+4] = x4;
            b[i+5] = x5;
            b[i+6] = x6;
            b[i+7] = x7;
        }

        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            float x = a_original[LEN_1D - i - 1] + b[i] * c[i];
            a[i] = x - 1.0f;
            b[i] = x;
        }
    }

    free(a_original);
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
                s281(iterations, LEN_1D, a_scalar, b_scalar, c_scalar); vectorized_s281(iterations, LEN_1D, a_vector, b_vector, c_vector);
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
