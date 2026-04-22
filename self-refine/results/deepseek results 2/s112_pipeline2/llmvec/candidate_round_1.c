#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s112(float *a, float *b, int iterations, int LEN_1D) {
    float *temp = (float*)malloc(LEN_1D * sizeof(float));

    for (int nl = 0; nl < 3 * iterations; nl++) {
        // Copy original 'a' values to temp before modification
        for (int i = 0; i < LEN_1D; i++) {
            temp[i] = a[i];
        }

        // Process all iterations in forward order using original values
        for (int i = 0; i < LEN_1D - 1; i++) {
            a[i + 1] = temp[i] + b[i];
        }
    }

    free(temp);
}

        #include <stdlib.h>
#include <string.h>

void vectorized_s112(float *a, float *b, int iterations, int LEN_1D) {
    float *temp = (float*)malloc(LEN_1D * sizeof(float));

    for (int nl = 0; nl < 3 * iterations; nl++) {
        // Copy original 'a' values to temp before modification
        memcpy(temp, a, LEN_1D * sizeof(float));

        // Process all iterations in forward order using original values
        // a[i+1] = temp[i] + b[i] for i in [0, LEN_1D-2]
        // This is: a[1..LEN_1D-1] = temp[0..LEN_1D-2] + b[0..LEN_1D-2]
        // No loop-carried dependency since we use temp[], so vectorizable.
        int i = 0;
        int n = LEN_1D - 1;

#if defined(__clang__) || defined(__GNUC__)
        typedef float float4 __attribute__((vector_size(16)));
        int vec_end = n - (n % 4);
        for (; i < vec_end; i += 4) {
            float4 t, bv, res;
            __builtin_memcpy(&t,  temp + i, 16);
            __builtin_memcpy(&bv, b    + i, 16);
            res = t + bv;
            __builtin_memcpy(a + i + 1, &res, 16);
        }
#endif
        // Scalar tail
        for (; i < n; i++) {
            a[i + 1] = temp[i] + b[i];
        }
    }

    free(temp);
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
            float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; int iterations = 5; int LEN_1D = arr_len;

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar));
                s112(a_scalar, b_scalar, iterations, LEN_1D); vectorized_s112(a_vector, b_vector, iterations, LEN_1D);
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
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
