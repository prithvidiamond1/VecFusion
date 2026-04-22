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

        // Loop distribution: separate the reduction from the store
        // First compute all prefix sums into a temporary array
        float temp_sum = sum;

        // Use manual unrolling to expose independent operations
        int i = 0;
        for (; i + 3 < LEN_1D; i += 4) {
            // Load all inputs first (no dependencies)
            float t0 = a[i];
            float t1 = a[i+1];
            float t2 = a[i+2];
            float t3 = a[i+3];

            // Compute prefix sums sequentially but with explicit dependency chain
            float s0 = temp_sum + t0;
            float s1 = s0 + t1;
            float s2 = s1 + t2;
            float s3 = s2 + t3;

            // Store all results
            b[i] = s0;
            b[i+1] = s1;
            b[i+2] = s2;
            b[i+3] = s3;

            // Update accumulator for next chunk
            temp_sum = s3;
        }

        // Handle remaining elements
        for (; i < LEN_1D; i++) {
            temp_sum += a[i];
            b[i] = temp_sum;
        }

        sum = temp_sum;
    }
    return sum;
}

        float vectorized_s3112(int iterations, int LEN_1D, float* a, float* b)
{
    float sum;
    for (int nl = 0; nl < iterations; nl++) {
        sum = (float)0.0;

        float temp_sum = sum;

        int i = 0;
        for (; i + 3 < LEN_1D; i += 4) {
            float t0 = a[i];
            float t1 = a[i+1];
            float t2 = a[i+2];
            float t3 = a[i+3];

            float s0 = temp_sum + t0;
            float s1 = s0 + t1;
            float s2 = s1 + t2;
            float s3 = s2 + t3;

            b[i]   = s0;
            b[i+1] = s1;
            b[i+2] = s2;
            b[i+3] = s3;

            temp_sum = s3;
        }

        for (; i < LEN_1D; i++) {
            temp_sum += a[i];
            b[i] = temp_sum;
        }

        sum = temp_sum;
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
