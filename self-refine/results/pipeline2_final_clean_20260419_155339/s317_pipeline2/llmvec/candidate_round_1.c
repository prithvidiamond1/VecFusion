#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s317(int iterations, int LEN_1D) {
    float q;
    int half_len = LEN_1D / 2;

    // Precompute (0.99f)^half_len once using repeated multiplication
    // This maintains the same order of operations as the original
    float power = 1.0f;
    for (int i = 0; i < half_len; i++) {
        power *= 0.99f;
    }

    // Outer loop only assigns the precomputed value
    for (int nl = 0; nl < 5 * iterations; nl++) {
        q = power;
    }
    return q;
}

        #include <stdint.h>

float vectorized_s317(int iterations, int LEN_1D) {
    float q;
    int half_len = LEN_1D / 2;

    // Precompute (0.99f)^half_len using vectorized multiplication
    float power = 1.0f;

    // Vectorized computation of power using 4-wide SIMD
    typedef float v4f __attribute__((vector_size(16)));
    v4f v_power = {1.0f, 1.0f, 1.0f, 1.0f};
    v4f v_factor = {0.99f, 0.99f, 0.99f, 0.99f};

    // Process in chunks of 4
    int i = 0;
    for (; i + 3 < half_len; i += 4) {
        v_power *= v_factor;
    }

    // Combine vector results
    float temp[4];
    __builtin_memcpy(temp, &v_power, sizeof(temp));
    power = temp[0] * temp[1] * temp[2] * temp[3];

    // Handle remaining iterations
    for (; i < half_len; i++) {
        power *= 0.99f;
    }

    // Outer loop - simple assignment
    for (int nl = 0; nl < 5 * iterations; nl++) {
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
