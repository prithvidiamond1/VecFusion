#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s112(float *a, float *b, int iterations, int LEN_1D) {
    for (int nl = 0; nl < 3 * iterations; nl++) {
        // Store original values to break dependency
        float temp[LEN_1D];
        for (int i = 0; i < LEN_1D - 1; i++) {
            temp[i] = a[i];
        }

        // Vectorizable loop - no loop-carried dependency
        for (int i = 0; i < LEN_1D - 1; i++) {
            a[i + 1] = temp[i] + b[i];
        }
    }
}

        #include <stddef.h>

void vectorized_s112(float *a, float *b, int iterations, int LEN_1D) {
    int outer_iterations = 3 * iterations;
    int len = LEN_1D - 1;

    for (int nl = 0; nl < outer_iterations; nl++) {
        // Store original values to break dependency
        float temp[LEN_1D];

        // Vectorized copy loop
        int i = 0;
        typedef float v4f __attribute__((vector_size(16)));
        const int VEC_SIZE = 4;
        int vec_limit = len - (len % VEC_SIZE);

        for (; i < vec_limit; i += VEC_SIZE) {
            v4f a_vec = *(v4f*)&a[i];
            *(v4f*)&temp[i] = a_vec;
        }

        // Scalar tail for copy
        for (; i < len; i++) {
            temp[i] = a[i];
        }

        // Vectorized computation loop
        i = 0;
        vec_limit = len - (len % VEC_SIZE);

        for (; i < vec_limit; i += VEC_SIZE) {
            v4f temp_vec = *(v4f*)&temp[i];
            v4f b_vec = *(v4f*)&b[i];
            v4f result_vec = temp_vec + b_vec;
            *(v4f*)&a[i + 1] = result_vec;
        }

        // Scalar tail for computation
        for (; i < len; i++) {
            a[i + 1] = temp[i] + b[i];
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
