#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        void s281(int iterations,int LEN_1D, float* a,float* b,float* c)
{
    float x;
    for (int nl = 0; nl < iterations; nl++) {
        for (int i = 0; i < LEN_1D; i++) {
            x = a[LEN_1D-i-1] + b[i] * c[i];
            a[i] = x-(float)1.0;
            b[i] = x;
        }
    }
}

        void vectorized_s281(int iterations, int LEN_1D, float* a, float* b, float* c)
{
    // Note: this loop has a loop-carried dependency through a[] (a[LEN_1D-i-1] reads
    // from positions that overlap with a[i] writes), so we must be careful.
    // For the general case, we preserve scalar semantics exactly.
    // However, we can unroll the inner loop manually for better instruction-level parallelism
    // when there is no aliasing overlap (i.e., when i and LEN_1D-i-1 don't overlap).
    // We split into: safe vectorizable region and the overlap region.

    for (int nl = 0; nl < iterations; nl++) {
        // The read index is LEN_1D-i-1, write index is i.
        // They overlap when i == LEN_1D-i-1, i.e., i == (LEN_1D-1)/2
        // For i < (LEN_1D-1)/2: read index > write index, safe to vectorize forward
        // For i > (LEN_1D-1)/2: read index < write index, already written, must be scalar
        // For i == (LEN_1D-1)/2: single element, scalar

        int safe_end = (LEN_1D - 1) / 2; // exclusive upper bound for safe region

        // Vectorized safe region using 4-wide unroll
        int i = 0;
        for (; i + 3 < safe_end; i += 4) {
            float x0 = a[LEN_1D - i - 1] + b[i] * c[i];
            float x1 = a[LEN_1D - (i+1) - 1] + b[i+1] * c[i+1];
            float x2 = a[LEN_1D - (i+2) - 1] + b[i+2] * c[i+2];
            float x3 = a[LEN_1D - (i+3) - 1] + b[i+3] * c[i+3];
            a[i]   = x0 - 1.0f;
            a[i+1] = x1 - 1.0f;
            a[i+2] = x2 - 1.0f;
            a[i+3] = x3 - 1.0f;
            b[i]   = x0;
            b[i+1] = x1;
            b[i+2] = x2;
            b[i+3] = x3;
        }
        // Scalar cleanup for safe region tail
        for (; i < safe_end; i++) {
            float x = a[LEN_1D - i - 1] + b[i] * c[i];
            a[i] = x - 1.0f;
            b[i] = x;
        }

        // Scalar region for overlap and beyond (reads from already-written a[])
        for (; i < LEN_1D; i++) {
            float x = a[LEN_1D - i - 1] + b[i] * c[i];
            a[i] = x - 1.0f;
            b[i] = x;
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
            int iterations = 5; int LEN_1D = n; float a_scalar[128]; float a_vector[128]; float b_scalar[128]; float b_vector[128]; float c_scalar[128]; float c_vector[128];

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, n, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar)); fill_f32(b_scalar, n, &seed); memcpy(b_vector, b_scalar, sizeof(b_scalar)); fill_f32(c_scalar, n, &seed); memcpy(c_vector, c_scalar, sizeof(c_scalar));
                s281(iterations, LEN_1D, a_scalar, b_scalar, c_scalar); vectorized_s281(iterations, LEN_1D, a_vector, b_vector, c_vector);
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
} for (int i = 0; i < n; ++i) {
    if (fabsf((c_scalar[i]) - (c_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter c on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
