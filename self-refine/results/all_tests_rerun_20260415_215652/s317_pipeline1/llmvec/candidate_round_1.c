#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s317(int iterations,int LEN_1D)
{
    float q;
    for (int nl = 0; nl < 5*iterations; nl++) {
        q = (float)1.;
        for (int i = 0; i < LEN_1D/2; i++) {
            q *= (float).99;
        }
    }
    return q;
}

        float vectorized_s317(int iterations, int LEN_1D)
{
    float q;
    for (int nl = 0; nl < 5 * iterations; nl++) {
        q = (float)1.;
        int half = LEN_1D / 2;

        // Use 4-wide unrolling: maintain 4 accumulators
        float q0 = 1.0f, q1 = 1.0f, q2 = 1.0f, q3 = 1.0f;
        int i = 0;
        int limit = half - (half % 4);
        for (; i < limit; i += 4) {
            q0 *= 0.99f;
            q1 *= 0.99f;
            q2 *= 0.99f;
            q3 *= 0.99f;
        }
        // Combine accumulators
        float combined = q0 * q1 * q2 * q3;
        // Scalar tail
        for (; i < half; i++) {
            combined *= 0.99f;
        }
        q = combined;
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
            const int n = 128;
            uint32_t seed = 7u;
            int iterations = 5; int LEN_1D = n; float ret_scalar; float ret_vector;

            for (int trial = 0; trial < 64; ++trial) {

                ret_scalar = s317(iterations, LEN_1D); ret_vector = vectorized_s317(iterations, LEN_1D); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
    fprintf(stderr, "Return mismatch on trial %d\n", trial);
    return 2;
}

            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
