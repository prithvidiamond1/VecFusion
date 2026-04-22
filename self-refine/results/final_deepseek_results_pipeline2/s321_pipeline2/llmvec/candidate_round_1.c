#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s321(int iterations, int LEN_1D, float* a, float* b) {
    for (int nl = 0; nl < iterations; nl++) {
        float prev = a[0];
        for (int i = 1; i < LEN_1D; i++) {
            float temp = a[i];
            a[i] = temp + prev * b[i];
            prev = a[i];
        }
    }
}

        typedef float float4 __attribute__((ext_vector_type(4)));

void vectorized_s321(int iterations, int LEN_1D, float* a, float* b) {
    for (int nl = 0; nl < iterations; nl++) {
        float prev = a[0];

        // Process first element separately to establish prev
        int i = 1;

        // Main vectorized loop
        for (; i + 3 < LEN_1D; i += 4) {
            // Load current block
            float4 a_vec = {a[i], a[i+1], a[i+2], a[i+3]};
            float4 b_vec = {b[i], b[i+1], b[i+2], b[i+3]};

            // Compute: a[i] = a[i] + prev * b[i]
            // Then prev becomes the new a[i] for next iteration
            float4 prev_vec = {prev, a[i], a[i+1], a[i+2]};
            float4 result = a_vec + prev_vec * b_vec;

            // Store results
            a[i] = result[0];
            a[i+1] = result[1];
            a[i+2] = result[2];
            a[i+3] = result[3];

            // Update prev for next iteration
            prev = result[3];
        }

        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            float temp = a[i];
            a[i] = temp + prev * b[i];
            prev = a[i];
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
            const int arr_len = 128;
            uint32_t seed = 7u;
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar));
                s321(iterations, LEN_1D, a_scalar, b_scalar); vectorized_s321(iterations, LEN_1D, a_vector, b_vector);
                for (int i = 0; i < arr_len; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((b_scalar[i]) - (b_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter b on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
