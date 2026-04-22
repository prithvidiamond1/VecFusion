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
        float temp_sum = (float)0.0;

        // Prefix sum loop (sequential dependency remains)
        for (int i = 0; i < LEN_1D; i++) {
            temp_sum += a[i];
            b[i] = temp_sum;
        }

        // Use the final temp_sum as sum (eliminates redundant second loop)
        sum = temp_sum;
    }
    return sum;
}

        #include <stdint.h>

float vectorized_s3112(int iterations, int LEN_1D, float* a, float* b)
{
    float sum;
    for (int nl = 0; nl < iterations; nl++) {
        float temp_sum = 0.0f;

        // The prefix sum has a sequential dependency: b[i] = a[0]+a[1]+...+a[i]
        // We can vectorize by computing partial sums in blocks, then fixup.
        // Block size 8 for unrolling.
        int i = 0;
        int block = 8;
        int limit = LEN_1D - (LEN_1D % block);

        for (; i < limit; i += block) {
            // Load 8 elements, compute prefix within block, then add carry
            float v0 = a[i+0];
            float v1 = a[i+1];
            float v2 = a[i+2];
            float v3 = a[i+3];
            float v4 = a[i+4];
            float v5 = a[i+5];
            float v6 = a[i+6];
            float v7 = a[i+7];

            // Prefix sum within block
            float p0 = temp_sum + v0;
            float p1 = p0 + v1;
            float p2 = p1 + v2;
            float p3 = p2 + v3;
            float p4 = p3 + v4;
            float p5 = p4 + v5;
            float p6 = p5 + v6;
            float p7 = p6 + v7;

            b[i+0] = p0;
            b[i+1] = p1;
            b[i+2] = p2;
            b[i+3] = p3;
            b[i+4] = p4;
            b[i+5] = p5;
            b[i+6] = p6;
            b[i+7] = p7;

            temp_sum = p7;
        }

        // Scalar cleanup tail
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
