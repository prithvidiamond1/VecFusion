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

        // Vectorizable reduction loop
        float temp_sum = sum;
        for (int i = 0; i < LEN_1D; i++) {
            temp_sum += a[i];
        }
        sum = temp_sum;

        // Vectorizable prefix sum using segmented approach
        float prefix = (float)0.0;
        const int CHUNK_SIZE = 4;
        int remainder = LEN_1D % CHUNK_SIZE;
        int chunks = LEN_1D / CHUNK_SIZE;

        // Process full chunks - each chunk computes independent prefix
        for (int chunk = 0; chunk < chunks; chunk++) {
            int base = chunk * CHUNK_SIZE;

            // Load chunk values
            float a0 = a[base];
            float a1 = a[base + 1];
            float a2 = a[base + 2];
            float a3 = a[base + 3];

            // Compute chunk-local prefix sums
            float chunk_prefix0 = a0;
            float chunk_prefix1 = a0 + a1;
            float chunk_prefix2 = a0 + a1 + a2;
            float chunk_prefix3 = a0 + a1 + a2 + a3;

            // Store chunk results with global offset
            b[base] = prefix + chunk_prefix0;
            b[base + 1] = prefix + chunk_prefix1;
            b[base + 2] = prefix + chunk_prefix2;
            b[base + 3] = prefix + chunk_prefix3;

            // Update global prefix for next chunk
            prefix += chunk_prefix3;
        }

        // Process remainder
        int base = chunks * CHUNK_SIZE;
        for (int i = 0; i < remainder; i++) {
            prefix += a[base + i];
            b[base + i] = prefix;
        }
    }
    return sum;
}

        #include <stddef.h>

float vectorized_s3112(int iterations, int LEN_1D, float* a, float* b)
{
    float sum;
    for (int nl = 0; nl < iterations; nl++) {
        sum = (float)0.0;

        // Vectorizable reduction loop
        float temp_sum = sum;
        int i;
        for (i = 0; i + 3 < LEN_1D; i += 4) {
            temp_sum += a[i];
            temp_sum += a[i + 1];
            temp_sum += a[i + 2];
            temp_sum += a[i + 3];
        }
        for (; i < LEN_1D; i++) {
            temp_sum += a[i];
        }
        sum = temp_sum;

        // Vectorizable prefix sum using segmented approach
        float prefix = (float)0.0;
        const int CHUNK_SIZE = 4;
        int remainder = LEN_1D % CHUNK_SIZE;
        int chunks = LEN_1D / CHUNK_SIZE;

        // Process full chunks - each chunk computes independent prefix
        for (int chunk = 0; chunk < chunks; chunk++) {
            int base = chunk * CHUNK_SIZE;

            // Load chunk values
            float a0 = a[base];
            float a1 = a[base + 1];
            float a2 = a[base + 2];
            float a3 = a[base + 3];

            // Compute chunk-local prefix sums
            float chunk_prefix0 = a0;
            float chunk_prefix1 = a0 + a1;
            float chunk_prefix2 = a0 + a1 + a2;
            float chunk_prefix3 = a0 + a1 + a2 + a3;

            // Store chunk results with global offset
            b[base] = prefix + chunk_prefix0;
            b[base + 1] = prefix + chunk_prefix1;
            b[base + 2] = prefix + chunk_prefix2;
            b[base + 3] = prefix + chunk_prefix3;

            // Update global prefix for next chunk
            prefix += chunk_prefix3;
        }

        // Process remainder
        int base = chunks * CHUNK_SIZE;
        for (int i = 0; i < remainder; i++) {
            prefix += a[base + i];
            b[base + i] = prefix;
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
