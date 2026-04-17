#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s342(int iterations,int LEN_1D, float* a, float *b)
{
    int j = 0;
    for (int nl = 0; nl < iterations; nl++) {
        j = -1;
        for (int i = 0; i < LEN_1D; i++) {
            if (a[i] > (float)0.) {
                j++;
                a[i] = b[j];
            }
        }
    }
}

        void vectorized_s342(int iterations, int LEN_1D, float* a, float *b)
{
    // This loop has a data-dependent index j that depends on runtime values of a[i],
    // making true SIMD vectorization of the inner loop semantically complex.
    // We unroll the scalar loop manually for better instruction-level parallelism
    // while preserving exact semantics.
    for (int nl = 0; nl < iterations; nl++) {
        int j = -1;
        int i = 0;

        // Unroll by 4 - but we must be careful: j increments conditionally,
        // so we can't fully vectorize without knowing the branch outcomes.
        // Instead, we unroll to help the compiler with pipelining.
        for (; i <= LEN_1D - 4; i += 4) {
            if (a[i] > 0.0f) {
                j++;
                a[i] = b[j];
            }
            if (a[i+1] > 0.0f) {
                j++;
                a[i+1] = b[j];
            }
            if (a[i+2] > 0.0f) {
                j++;
                a[i+2] = b[j];
            }
            if (a[i+3] > 0.0f) {
                j++;
                a[i+3] = b[j];
            }
        }

        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            if (a[i] > 0.0f) {
                j++;
                a[i] = b[j];
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
            const int n = 128;
            uint32_t seed = 7u;
            int iterations = 5; int LEN_1D = n; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, n, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar));
                s342(iterations, LEN_1D, a_scalar, b_scalar); vectorized_s342(iterations, LEN_1D, a_vector, b_vector);
                for (int i = 0; i < n; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((b_scalar[i]) - (b_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter b on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
