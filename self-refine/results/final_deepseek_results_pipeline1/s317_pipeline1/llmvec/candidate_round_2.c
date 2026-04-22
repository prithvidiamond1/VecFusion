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

        typedef float float32 __attribute__((ext_vector_type(4)));

float vectorized_s317(int iterations, int LEN_1D) {
    float q = 1.0f;
    int total_outer = 5 * iterations;
    int inner_len = LEN_1D;

    // Vectorized inner loop: process 4 iterations at a time
    float32 vq = {1.0f, 1.0f, 1.0f, 1.0f};
    float32 vfactor = {0.99f, 0.99f, 0.99f, 0.99f};
    int i = 0;
    for (; i + 4 <= inner_len; i += 4) {
        vq *= vfactor;
    }
    // Reduce vector lanes to a single scalar
    float scalar_q = vq[0] * vq[1] * vq[2] * vq[3];
    // Scalar cleanup for remaining inner iterations
    for (; i < inner_len; i++) {
        scalar_q *= 0.99f;
    }

    // Outer loop: multiply q by scalar_q for each outer iteration
    for (int nl = 0; nl < total_outer; nl++) {
        q *= scalar_q;
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
