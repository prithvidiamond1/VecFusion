#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s3112(int iterations, int LEN_1D, float* a, float* b)
{
    float sum;
    for (int nl = 0; nl < iterations; nl++) {
        sum = (float)0.0;

        // First loop: compute reduction (vectorizable)
        float temp_sum = (float)0.0;
        for (int i = 0; i < LEN_1D; i++) {
            temp_sum += a[i];
        }
        sum = temp_sum;

        // Second loop: compute prefix sum with reordered memory access
        // Store original a[i] before updating prefix_sum to break dependency
        float prefix_sum = (float)0.0;
        for (int i = 0; i < LEN_1D; i++) {
            float current_a = a[i];  // Load a[i] first
            prefix_sum += current_a; // Then update prefix_sum
            b[i] = prefix_sum;       // Finally store to b[i]
        }
    }
    return sum;
}

        #include <stddef.h>

float vectorized_s3112(int iterations, int LEN_1D, float* a, float* b)
{
    float sum = 0.0f;

    for (int nl = 0; nl < iterations; nl++) {
        // Vectorized reduction
        float temp_sum = 0.0f;
        int i = 0;

        // Process in chunks of 4 for better vectorization potential
        for (; i + 3 < LEN_1D; i += 4) {
            temp_sum += a[i] + a[i+1] + a[i+2] + a[i+3];
        }

        // Scalar tail
        for (; i < LEN_1D; i++) {
            temp_sum += a[i];
        }

        sum = temp_sum;

        // Prefix sum - use explicit dependency breaking
        float prefix_sum = 0.0f;
        i = 0;

        // Process in chunks of 4 with explicit unrolling
        for (; i + 3 < LEN_1D; i += 4) {
            float a0 = a[i];
            float a1 = a[i+1];
            float a2 = a[i+2];
            float a3 = a[i+3];

            prefix_sum += a0;
            b[i] = prefix_sum;

            prefix_sum += a1;
            b[i+1] = prefix_sum;

            prefix_sum += a2;
            b[i+2] = prefix_sum;

            prefix_sum += a3;
            b[i+3] = prefix_sum;
        }

        // Scalar tail for prefix sum
        for (; i < LEN_1D; i++) {
            float current_a = a[i];
            prefix_sum += current_a;
            b[i] = prefix_sum;
        }
    }

    return sum;
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float ret_scalar; float ret_vector;

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar));
                ret_scalar = s3112(iterations, LEN_1D, a_scalar, b_scalar); ret_vector = vectorized_s3112(iterations, LEN_1D, a_vector, b_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
    fprintf(stderr, "Return mismatch on trial %d\n", trial);
    return 2;
}
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
