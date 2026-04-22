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

        #include <stdint.h>

float vectorized_s312(int iterations, int LEN_1D, float* a)
{
    float prod;
    for (int nl = 0; nl < 10 * iterations; nl++) {
        // Use 4-wide unrolling to allow the compiler to vectorize the reduction
        float prod0 = 1.0f;
        float prod1 = 1.0f;
        float prod2 = 1.0f;
        float prod3 = 1.0f;

        int i = 0;
        int limit = LEN_1D - (LEN_1D % 4);
        for (; i < limit; i += 4) {
            prod0 *= a[i + 0];
            prod1 *= a[i + 1];
            prod2 *= a[i + 2];
            prod3 *= a[i + 3];
        }
        // Combine partial products
        prod0 *= prod1;
        prod2 *= prod3;
        prod0 *= prod2;

        // Scalar tail
        for (; i < LEN_1D; i++) {
            prod0 *= a[i];
        }

        prod = prod0;
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
            const int n = 128;
            uint32_t seed = 7u;
            int iterations = 5; int LEN_1D = n; float a_scalar[128]; float a_vector[128]; float ret_scalar; float ret_vector;

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar));
                ret_scalar = s312(iterations, LEN_1D, a_scalar); ret_vector = vectorized_s312(iterations, LEN_1D, a_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
    fprintf(stderr, "Return mismatch on trial %d\n", trial);
    return 2;
}
                for (int i = 0; i < n; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
