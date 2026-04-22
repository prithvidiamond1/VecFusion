#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s352(int iterations,int LEN_1D, float* a, float *b)
{
    float dot;
    for (int nl = 0; nl < 8*iterations; nl++) {
        dot = 0.0f;

        // Process elements in groups of 5 (matching original stride)
        float dot0 = 0.0f, dot1 = 0.0f, dot2 = 0.0f, dot3 = 0.0f, dot4 = 0.0f;

        int i;
        for (i = 0; i <= LEN_1D - 5; i += 5) {
            // Load all values first (enables better vectorization)
            float a0 = a[i], a1 = a[i+1], a2 = a[i+2], a3 = a[i+3], a4 = a[i+4];
            float b0 = b[i], b1 = b[i+1], b2 = b[i+2], b3 = b[i+3], b4 = b[i+4];

            // Compute all products (independent operations)
            float prod0 = a0 * b0;
            float prod1 = a1 * b1;
            float prod2 = a2 * b2;
            float prod3 = a3 * b3;
            float prod4 = a4 * b4;

            // Accumulate into separate variables (no loop-carried dependency)
            dot0 += prod0;
            dot1 += prod1;
            dot2 += prod2;
            dot3 += prod3;
            dot4 += prod4;
        }

        // Combine partial sums (reduces dependency chain)
        dot = ((dot0 + dot1) + (dot2 + dot3)) + dot4;

        // Handle remaining elements (0-4 elements) - matches original out-of-bounds behavior
        for (; i < LEN_1D; i++) {
            dot += a[i] * b[i];
        }
        // Add out-of-bounds contributions to match original buggy behavior
        for (int j = i; j < i + 4 && j < LEN_1D + 4; j++) {
            if (j >= LEN_1D) {
                dot += a[j] * b[j];  // This matches original out-of-bounds access
            }
        }
    }
    return dot;
}

        float vectorized_s352(int iterations, int LEN_1D, float* a, float *b) {
    float dot;

    for (int nl = 0; nl < 8 * iterations; nl++) {
        dot = 0.0f;

        typedef float v4f __attribute__((vector_size(16)));
        v4f vdot0 = {0.0f, 0.0f, 0.0f, 0.0f};
        v4f vdot1 = {0.0f, 0.0f, 0.0f, 0.0f};

        int i;
        for (i = 0; i <= LEN_1D - 8; i += 8) {
            v4f va0 = *(v4f*)&a[i];
            v4f vb0 = *(v4f*)&b[i];
            v4f va1 = *(v4f*)&a[i + 4];
            v4f vb1 = *(v4f*)&b[i + 4];

            vdot0 += va0 * vb0;
            vdot1 += va1 * vb1;
        }

        float* vdot0p = (float*)&vdot0;
        float* vdot1p = (float*)&vdot1;
        dot = (vdot0p[0] + vdot0p[1] + vdot0p[2] + vdot0p[3]) +
              (vdot1p[0] + vdot1p[1] + vdot1p[2] + vdot1p[3]);

        for (; i < LEN_1D; i++) {
            dot += a[i] * b[i];
        }
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
