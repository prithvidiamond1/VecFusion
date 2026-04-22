#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s112(float *a, float *b, int iterations, int LEN_1D) {
    int total_iters = 3 * iterations;

    for (int nl = 0; nl < total_iters; nl++) {
        // Process in backward direction to preserve dependency
        // No loop splitting needed as each iteration is independent in backward order
        for (int i = LEN_1D - 2; i >= 0; i--) {
            float temp = a[i] + b[i];
            a[i + 1] = temp;
        }
    }
}

        typedef float float4 __attribute__((ext_vector_type(4)));

void vectorized_s112(float *a, float *b, int iterations, int LEN_1D) {
    int total_iters = 3 * iterations;

    for (int nl = 0; nl < total_iters; nl++) {
        // Vectorized backward processing
        int i = LEN_1D - 2;

        // Process vector chunks
        for (; i >= 3; i -= 4) {
            float4 a_vec = (float4){a[i-3], a[i-2], a[i-1], a[i]};
            float4 b_vec = (float4){b[i-3], b[i-2], b[i-1], b[i]};
            float4 temp_vec = a_vec + b_vec;

            // Store results shifted by +1 position
            a[i-2] = temp_vec[0];
            a[i-1] = temp_vec[1];
            a[i] = temp_vec[2];
            a[i+1] = temp_vec[3];
        }

        // Scalar cleanup for remaining elements
        for (; i >= 0; i--) {
            float temp = a[i] + b[i];
            a[i + 1] = temp;
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
