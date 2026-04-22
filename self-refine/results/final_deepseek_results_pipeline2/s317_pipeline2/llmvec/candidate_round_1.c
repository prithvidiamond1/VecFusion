#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s317(int iterations, int LEN_1D) {
    float q;
    int half_len = LEN_1D / 2;
    float factor = (float)0.99;

    for (int nl = 0; nl < 5 * iterations; nl++) {
        q = (float)1.0;
        float temp = q;

        for (int i = 0; i < half_len; i++) {
            temp *= factor;
        }

        q = temp;
    }

    return q;
}

        typedef float float4 __attribute__((ext_vector_type(4)));

float vectorized_s317(int iterations, int LEN_1D) {
    float q;
    int half_len = LEN_1D / 2;
    float factor = (float)0.99;

    for (int nl = 0; nl < 5 * iterations; nl++) {
        q = (float)1.0;
        float temp = q;

        // Vectorized multiplication
        int i = 0;
        float4 v_factor = (float4){factor, factor, factor, factor};
        float4 v_temp = (float4){temp, temp, temp, temp};

        // Process 4 elements at a time
        for (; i + 3 < half_len; i += 4) {
            v_temp *= v_factor;
        }

        // Horizontal reduction of vector accumulator
        temp = v_temp.x * v_temp.y * v_temp.z * v_temp.w;

        // Scalar tail
        for (; i < half_len; i++) {
            temp *= factor;
        }

        q = temp;
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
