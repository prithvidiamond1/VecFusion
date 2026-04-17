#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s317(int iterations, int LEN_1D) {
    float q = (float)1.;
    for (int i = 0; i < LEN_1D/2; i++) {
        q *= (float).99;
    }
    return q;
}

        #include <stdint.h>

float vectorized_s317(int iterations, int LEN_1D) {
    int n = LEN_1D / 2;

    // Use 8-wide unrolling to allow the compiler to vectorize
    float q0 = 1.0f, q1 = 1.0f, q2 = 1.0f, q3 = 1.0f;
    float q4 = 1.0f, q5 = 1.0f, q6 = 1.0f, q7 = 1.0f;

    const float factor = 0.99f;

    int i = 0;
    int n8 = n - (n % 8);

    for (; i < n8; i += 8) {
        q0 *= factor;
        q1 *= factor;
        q2 *= factor;
        q3 *= factor;
        q4 *= factor;
        q5 *= factor;
        q6 *= factor;
        q7 *= factor;
    }

    // Combine partial results
    float q = q0 * q1 * q2 * q3 * q4 * q5 * q6 * q7;

    // Scalar cleanup tail
    for (; i < n; i++) {
        q *= factor;
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
