#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s317(int iterations, int LEN_1D) {
    float q = 1.0f;
    int inner_iters = LEN_1D / 2;

    // Handle edge case where iterations = 0
    if (iterations == 0) {
        return q; // Return uninitialized-like value (1.0f matches original after first inner loop)
    }

    // Loop distribution: compute power once outside outer loop
    float power = 1.0f;

    // Vectorizable reduction loop (no loop-carried dependency after reorganization)
    for (int i = 0; i < inner_iters; i++) {
        power *= 0.99f;
    }

    // Outer loop simplified (no inner computation)
    for (int nl = 0; nl < 5 * iterations; nl++) {
        q = power;
    }

    return q;
}

        #include <math.h>

float vectorized_s317(int iterations, int LEN_1D) {
    float q = 1.0f;
    int inner_iters = LEN_1D / 2;

    if (iterations == 0) {
        return q;
    }

    float power = 1.0f;

    // Vectorized reduction: compute 0.99^inner_iters
    // Use 4-wide unrolling with independent accumulators
    float acc0 = 1.0f, acc1 = 1.0f, acc2 = 1.0f, acc3 = 1.0f;
    int i = 0;
    int limit = inner_iters - (inner_iters % 4);

    for (; i < limit; i += 4) {
        acc0 *= 0.99f;
        acc1 *= 0.99f;
        acc2 *= 0.99f;
        acc3 *= 0.99f;
    }

    // Combine accumulators
    power = acc0 * acc1 * acc2 * acc3;

    // Scalar tail
    for (; i < inner_iters; i++) {
        power *= 0.99f;
    }

    // Outer loop: just assign power to q repeatedly
    // Unroll 4x for vectorization hint
    int outer_iters = 5 * iterations;
    int nl = 0;
    int outer_limit = outer_iters - (outer_iters % 4);

    for (; nl < outer_limit; nl += 4) {
        q = power;
        q = power;
        q = power;
        q = power;
    }
    for (; nl < outer_iters; nl++) {
        q = power;
    }

    return q;
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
            int iterations = 5; int LEN_1D = arr_len; float ret_scalar; float ret_vector;

            for (int trial = 0; trial < 64; ++trial) {

                ret_scalar = s317(iterations, LEN_1D); ret_vector = vectorized_s317(iterations, LEN_1D); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
    fprintf(stderr, "Return mismatch on trial %d\n", trial);
    return 2;
}

            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
