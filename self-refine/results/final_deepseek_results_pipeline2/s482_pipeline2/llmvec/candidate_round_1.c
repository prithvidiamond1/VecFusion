#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s482(int iterations, int LEN_1D, float* a, float *b, float *c) {
    for (int nl = 0; nl < iterations; nl++) {
        // First, find where the break condition occurs
        int break_idx = LEN_1D;
        #pragma clang loop vectorize(disable)
        for (int i = 0; i < LEN_1D; i++) {
            if (c[i] > b[i]) {
                break_idx = i;
                break;
            }
        }

        // Process all iterations before the break point (these can be vectorized)
        // Note: break_idx iteration is included because the break happens AFTER the update
        for (int i = 0; i <= break_idx && i < LEN_1D; i++) {
            a[i] += b[i] * c[i];
        }
    }
}

        #include <stdint.h>

void vectorized_s482(int iterations, int LEN_1D, float* a, float *b, float *c) {
    for (int nl = 0; nl < iterations; nl++) {
        // First, find where the break condition occurs
        int break_idx = LEN_1D;
        for (int i = 0; i < LEN_1D; i++) {
            if (c[i] > b[i]) {
                break_idx = i;
                break;
            }
        }

        // Process all iterations before the break point (these can be vectorized)
        // Note: break_idx iteration is included because the break happens AFTER the update
        int limit = break_idx + 1;
        if (limit > LEN_1D) limit = LEN_1D;

        // Vectorizable loop with explicit bounds
        int i = 0;
        #if defined(__clang__) && defined(__SSE__)
        // Process 4 elements at a time using vector extensions
        for (; i + 3 < limit; i += 4) {
            float va0 = a[i], va1 = a[i+1], va2 = a[i+2], va3 = a[i+3];
            float vb0 = b[i], vb1 = b[i+1], vb2 = b[i+2], vb3 = b[i+3];
            float vc0 = c[i], vc1 = c[i+1], vc2 = c[i+2], vc3 = c[i+3];
            a[i]   = va0   + vb0   * vc0;
            a[i+1] = va1   + vb1   * vc1;
            a[i+2] = va2   + vb2   * vc2;
            a[i+3] = va3   + vb3   * vc3;
        }
        #endif
        // Scalar cleanup for remaining elements
        for (; i < limit; i++) {
            a[i] += b[i] * c[i];
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, arr_len, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, arr_len, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar));
                s482(iterations, LEN_1D, a_scalar, b_scalar, c_scalar); vectorized_s482(iterations, LEN_1D, a_vector, b_vector, c_vector);
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
} for (int i = 0; i < arr_len; ++i) {
    if (fabsf((c_scalar[i]) - (c_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter c on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
