#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s312(int iterations, int LEN_1D, float* a)
{
    float prod;
    int total_outer = 10 * iterations;

    for (int nl = 0; nl < total_outer; nl++) {
        prod = 1.0f;

        // Loop distribution: separate reduction from potential other operations
        // Here we only have reduction, so we keep it as is but ensure
        // the reduction variable is properly initialized each outer iteration

        // The inner loop has loop-carried dependency (prod depends on previous iteration)
        // This prevents vectorization of the reduction itself
        // However, we can ensure the loop structure is clean for potential
        // compiler optimizations like unrolling

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
    int total_outer = 10 * iterations;

    for (int nl = 0; nl < total_outer; nl++) {
        prod = 1.0f;

        // Unroll by 8 to allow better pipelining/vectorization
        float p0 = 1.0f, p1 = 1.0f, p2 = 1.0f, p3 = 1.0f;
        float p4 = 1.0f, p5 = 1.0f, p6 = 1.0f, p7 = 1.0f;

        int i = 0;
        int limit = LEN_1D - (LEN_1D % 8);

        for (; i < limit; i += 8) {
            p0 *= a[i + 0];
            p1 *= a[i + 1];
            p2 *= a[i + 2];
            p3 *= a[i + 3];
            p4 *= a[i + 4];
            p5 *= a[i + 5];
            p6 *= a[i + 6];
            p7 *= a[i + 7];
        }

        // Combine partial products
        prod = p0 * p1 * p2 * p3 * p4 * p5 * p6 * p7;

        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            prod *= a[i];
        }
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
