#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s293(int iterations,int LEN_1D, float* a)
{
    for (int nl = 0; nl < 4*iterations; nl++) {
        for (int i = 0; i < LEN_1D; i++) {
            a[i] = a[0];
        }
    }
}

        #include <string.h>

void vectorized_s293(int iterations, int LEN_1D, float* a)
{
    for (int nl = 0; nl < 4 * iterations; nl++) {
        float val = a[0];
        int i = 0;
        // Vector-friendly unrolled loop: process 8 elements at a time
        for (; i <= LEN_1D - 8; i += 8) {
            a[i + 0] = val;
            a[i + 1] = val;
            a[i + 2] = val;
            a[i + 3] = val;
            a[i + 4] = val;
            a[i + 5] = val;
            a[i + 6] = val;
            a[i + 7] = val;
        }
        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            a[i] = val;
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar));
                s293(iterations, LEN_1D, a_scalar); vectorized_s293(iterations, LEN_1D, a_vector);
                for (int i = 0; i < arr_len; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
