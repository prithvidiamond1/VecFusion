#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s352(int iterations,int LEN_1D, float* a, float *b) {
    float dot;
    for (int nl = 0; nl < 8*iterations; nl++) {
        float dot0 = 0.0f, dot1 = 0.0f, dot2 = 0.0f, dot3 = 0.0f, dot4 = 0.0f;
        int i;
        int limit = LEN_1D - 4;

        // Process complete groups of 5
        for (i = 0; i < limit; i += 5) {
            float t0 = a[i] * b[i];
            float t1 = a[i + 1] * b[i + 1];
            float t2 = a[i + 2] * b[i + 2];
            float t3 = a[i + 3] * b[i + 3];
            float t4 = a[i + 4] * b[i + 4];

            dot0 += t0;
            dot1 += t1;
            dot2 += t2;
            dot3 += t3;
            dot4 += t4;
        }

        // Handle remaining elements (1-4)
        for (; i < LEN_1D; i++) {
            dot0 += a[i] * b[i];
        }

        dot = dot0 + dot1 + dot2 + dot3 + dot4;
    }
    return dot;
}

        #include <stdint.h>

float vectorized_s352(int iterations, int LEN_1D, float* a, float *b) {
    float dot = 0.0f;
    for (int nl = 0; nl < 8 * iterations; nl++) {
        float dot0 = 0.0f, dot1 = 0.0f, dot2 = 0.0f, dot3 = 0.0f;
        int i = 0;
        int limit4 = LEN_1D & ~3;

        // Process groups of 4 using vector-friendly unrolling
        for (i = 0; i < limit4; i += 4) {
            dot0 += a[i]     * b[i];
            dot1 += a[i + 1] * b[i + 1];
            dot2 += a[i + 2] * b[i + 2];
            dot3 += a[i + 3] * b[i + 3];
        }

        // Scalar cleanup for remaining elements
        for (; i < LEN_1D; i++) {
            dot0 += a[i] * b[i];
        }

        dot = dot0 + dot1 + dot2 + dot3;
    }
    return dot;
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
                ret_scalar = s352(iterations, LEN_1D, a_scalar, b_scalar); ret_vector = vectorized_s352(iterations, LEN_1D, a_vector, b_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
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
