#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s293(int iterations,int LEN_1D, float* a)
{
    float temp = a[0];
    int total_outer = 4 * iterations;

    for (int nl = 0; nl < total_outer; nl++) {
        for (int i = 0; i < LEN_1D; i++) {
            a[i] = temp;
        }
    }
}

        #include <string.h>

void vectorized_s293(int iterations, int LEN_1D, float* a)
{
    float temp = a[0];
    int total_outer = 4 * iterations;

    for (int nl = 0; nl < total_outer; nl++) {
        int i = 0;
        // Vectorized fill using 4-wide float vector
        typedef float float4 __attribute__((vector_size(16)));
        float4 vtemp = {temp, temp, temp, temp};
        float4* va = (float4*)__builtin_assume_aligned(a, 16);
        int vec_len = LEN_1D / 4;
        for (int vi = 0; vi < vec_len; vi++) {
            va[vi] = vtemp;
        }
        // Scalar tail
        for (i = vec_len * 4; i < LEN_1D; i++) {
            a[i] = temp;
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
