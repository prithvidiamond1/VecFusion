#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s341(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Pass 1: Compute mask array (vectorizable - no dependencies)
        int mask[LEN_1D];
        for (int i = 0; i < LEN_1D; i++) {
            mask[i] = (b[i] > (float)0.) ? 1 : 0;
        }

        // Pass 2: Compute prefix sum to get output indices
        int prefix[LEN_1D];
        prefix[0] = mask[0] - 1;
        for (int i = 1; i < LEN_1D; i++) {
            prefix[i] = prefix[i-1] + mask[i];
        }

        // Pass 3: Write results using prefix as index directly
        for (int i = 0; i < LEN_1D; i++) {
            if (mask[i]) {
                a[prefix[i]] = b[i];
            }
        }
    }
}

        #include <string.h>

void vectorized_s341(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Pass 1: Compute mask array using unrolled/vector-friendly code
        int mask[LEN_1D];

        int i = 0;
        // Unroll by 8 for vectorization
        for (; i <= LEN_1D - 8; i += 8) {
            mask[i+0] = (b[i+0] > 0.f) ? 1 : 0;
            mask[i+1] = (b[i+1] > 0.f) ? 1 : 0;
            mask[i+2] = (b[i+2] > 0.f) ? 1 : 0;
            mask[i+3] = (b[i+3] > 0.f) ? 1 : 0;
            mask[i+4] = (b[i+4] > 0.f) ? 1 : 0;
            mask[i+5] = (b[i+5] > 0.f) ? 1 : 0;
            mask[i+6] = (b[i+6] > 0.f) ? 1 : 0;
            mask[i+7] = (b[i+7] > 0.f) ? 1 : 0;
        }
        for (; i < LEN_1D; i++) {
            mask[i] = (b[i] > 0.f) ? 1 : 0;
        }

        // Pass 2: Compute prefix sum (sequential dependency, keep scalar)
        int prefix[LEN_1D];
        prefix[0] = mask[0] - 1;
        for (i = 1; i < LEN_1D; i++) {
            prefix[i] = prefix[i-1] + mask[i];
        }

        // Pass 3: Write results using prefix as index
        // Unroll by 8 for better throughput
        i = 0;
        for (; i <= LEN_1D - 8; i += 8) {
            if (mask[i+0]) a[prefix[i+0]] = b[i+0];
            if (mask[i+1]) a[prefix[i+1]] = b[i+1];
            if (mask[i+2]) a[prefix[i+2]] = b[i+2];
            if (mask[i+3]) a[prefix[i+3]] = b[i+3];
            if (mask[i+4]) a[prefix[i+4]] = b[i+4];
            if (mask[i+5]) a[prefix[i+5]] = b[i+5];
            if (mask[i+6]) a[prefix[i+6]] = b[i+6];
            if (mask[i+7]) a[prefix[i+7]] = b[i+7];
        }
        for (; i < LEN_1D; i++) {
            if (mask[i]) {
                a[prefix[i]] = b[i];
            }
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
            const int n = 128;
            uint32_t seed = 7u;
            int iterations = 5; int LEN_1D = n; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, n, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar));
                s341(iterations, LEN_1D, a_scalar, b_scalar); vectorized_s341(iterations, LEN_1D, a_vector, b_vector);
                for (int i = 0; i < n; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
} for (int i = 0; i < n; ++i) {
    if (fabsf((b_scalar[i]) - (b_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter b on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
