#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s341(int iterations,int LEN_1D, float* a, float *b)
{
    int j;
    for (int nl = 0; nl < iterations; nl++) {
        j = -1;
        for (int i = 0; i < LEN_1D; i++) {
            if (b[i] > (float)0.) {
                j++;
                a[j] = b[i];
            }
        }
    }
}

        #include <stddef.h>
#include <stdint.h>

typedef float float32_t __attribute__((ext_vector_type(4)));

void vectorized_s341(int iterations, int LEN_1D, float* a, float *b)
{
    for (int nl = 0; nl < iterations; nl++) {
        int j = -1;
        int i = 0;

        // Vectorized main loop
        for (; i + 4 <= LEN_1D; i += 4) {
            float32_t vb = *(float32_t*)(b + i);
            float32_t zero = (float32_t){0.0f, 0.0f, 0.0f, 0.0f};
            float32_t mask = vb > zero;

            // Extract mask bits
            int mask_bits = __builtin_convertvector(mask, int32_t);

            // Process each element
            for (int k = 0; k < 4; k++) {
                if (mask_bits & (1 << k)) {
                    j++;
                    a[j] = b[i + k];
                }
            }
        }

        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            if (b[i] > 0.0f) {
                j++;
                a[j] = b[i];
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
