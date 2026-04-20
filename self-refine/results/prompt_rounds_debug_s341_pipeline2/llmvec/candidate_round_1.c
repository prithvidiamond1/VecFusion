#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s341(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < iterations; nl++) {
        int write_idx = 0;
        for (int i = 0; i < LEN_1D; i++) {
            float temp = b[i];
            int mask = temp > (float)0.;
            a[write_idx] = temp;
            write_idx += mask;
        }
    }
}

        #include <stddef.h>

void vectorized_s341(int iterations, int LEN_1D, float* a, float* b) {
    for (int nl = 0; nl < iterations; nl++) {
        int write_idx = 0;
        int i = 0;

        // Process elements in chunks of 4 for potential vectorization
        for (; i + 3 < LEN_1D; i += 4) {
            float temp0 = b[i];
            float temp1 = b[i + 1];
            float temp2 = b[i + 2];
            float temp3 = b[i + 3];

            int mask0 = temp0 > 0.0f;
            int mask1 = temp1 > 0.0f;
            int mask2 = temp2 > 0.0f;
            int mask3 = temp3 > 0.0f;

            // Store all values first
            a[write_idx] = temp0;
            write_idx += mask0;
            a[write_idx] = temp1;
            write_idx += mask1;
            a[write_idx] = temp2;
            write_idx += mask2;
            a[write_idx] = temp3;
            write_idx += mask3;
        }

        // Scalar cleanup for remaining elements
        for (; i < LEN_1D; i++) {
            float temp = b[i];
            int mask = temp > 0.0f;
            a[write_idx] = temp;
            write_idx += mask;
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar));
                s341(iterations, LEN_1D, a_scalar, b_scalar); vectorized_s341(iterations, LEN_1D, a_vector, b_vector);
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
