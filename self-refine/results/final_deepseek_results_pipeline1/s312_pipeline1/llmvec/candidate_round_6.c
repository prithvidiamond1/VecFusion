#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s312(int iterations,int LEN_1D, float* a)
{

    float prod;
    for (int nl = 0; nl < 10*iterations; nl++) {
        prod = (float)1.;
        for (int i = 0; i < LEN_1D; i++) {
            prod *= a[i];
        }
    }
    return prod;
}

        #include <stddef.h>

typedef float v4sf __attribute__((__vector_size__(16)));

float vectorized_s312(int iterations, int LEN_1D, float* a) {
    float prod = 1.0f;
    int total_outer = 10 * iterations;

    for (int nl = 0; nl < total_outer; nl++) {
        v4sf vprod = {1.0f, 1.0f, 1.0f, 1.0f};
        int i = 0;

        if (LEN_1D >= 4) {
            for (; i <= LEN_1D - 4; i += 4) {
                v4sf va;
                __builtin_memcpy(&va, &a[i], sizeof(v4sf));
                vprod = va * vprod;
            }
        }

        float prod_scalar = 1.0f;
        for (; i < LEN_1D; i++) {
            prod_scalar *= a[i];
        }

        prod = prod_scalar * vprod[0] * vprod[1] * vprod[2] * vprod[3];
    }

    return prod;
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float ret_scalar; float ret_vector;

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar));
                ret_scalar = s312(iterations, LEN_1D, a_scalar); ret_vector = vectorized_s312(iterations, LEN_1D, a_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
    fprintf(stderr, "Return mismatch on trial %d\n", trial);
    return 2;
}
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
