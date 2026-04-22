#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s341(int iterations, int LEN_1D, float* a, float* b) {
    for (int nl = 0; nl < iterations; nl++) {
        int j = -1;
        // First loop: compute prefix sum of positive elements
        int prefix[LEN_1D];
        for (int i = 0; i < LEN_1D; i++) {
            prefix[i] = (b[i] > (float)0.) ? 1 : 0;
            if (i > 0) {
                prefix[i] += prefix[i-1];
            }
        }

        // Second loop: parallel copy using computed indices
        for (int i = 0; i < LEN_1D; i++) {
            if (b[i] > (float)0.) {
                a[prefix[i]-1] = b[i];
            }
        }
    }
}

        #include <string.h>

void vectorized_s341(int iterations, int LEN_1D, float* a, float* b) {
    for (int nl = 0; nl < iterations; nl++) {
        // Allocate prefix array on stack if small, otherwise use a local approach
        // We'll use a VLA for generality
        int prefix[LEN_1D];

        // Vectorize the prefix mask computation (parallel part)
        // Step 1: compute the indicator values in parallel
        int i;
        // Unroll by 8 for vector-friendly code
        int limit8 = (LEN_1D / 8) * 8;
        for (i = 0; i < limit8; i += 8) {
            prefix[i+0] = (b[i+0] > 0.0f) ? 1 : 0;
            prefix[i+1] = (b[i+1] > 0.0f) ? 1 : 0;
            prefix[i+2] = (b[i+2] > 0.0f) ? 1 : 0;
            prefix[i+3] = (b[i+3] > 0.0f) ? 1 : 0;
            prefix[i+4] = (b[i+4] > 0.0f) ? 1 : 0;
            prefix[i+5] = (b[i+5] > 0.0f) ? 1 : 0;
            prefix[i+6] = (b[i+6] > 0.0f) ? 1 : 0;
            prefix[i+7] = (b[i+7] > 0.0f) ? 1 : 0;
        }
        for (; i < LEN_1D; i++) {
            prefix[i] = (b[i] > 0.0f) ? 1 : 0;
        }

        // Step 2: sequential prefix sum (inherently sequential)
        for (i = 1; i < LEN_1D; i++) {
            prefix[i] += prefix[i-1];
        }

        // Step 3: scatter - unroll by 8
        for (i = 0; i < limit8; i += 8) {
            if (b[i+0] > 0.0f) a[prefix[i+0]-1] = b[i+0];
            if (b[i+1] > 0.0f) a[prefix[i+1]-1] = b[i+1];
            if (b[i+2] > 0.0f) a[prefix[i+2]-1] = b[i+2];
            if (b[i+3] > 0.0f) a[prefix[i+3]-1] = b[i+3];
            if (b[i+4] > 0.0f) a[prefix[i+4]-1] = b[i+4];
            if (b[i+5] > 0.0f) a[prefix[i+5]-1] = b[i+5];
            if (b[i+6] > 0.0f) a[prefix[i+6]-1] = b[i+6];
            if (b[i+7] > 0.0f) a[prefix[i+7]-1] = b[i+7];
        }
        for (; i < LEN_1D; i++) {
            if (b[i] > 0.0f) a[prefix[i]-1] = b[i];
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
                s341(iterations, LEN_1D, a_scalar, b_scalar); vectorized_s341(iterations, LEN_1D, a_vector, b_vector);
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
