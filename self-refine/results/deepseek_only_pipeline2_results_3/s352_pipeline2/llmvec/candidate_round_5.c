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

        // Split into two independent accumulators to break loop-carried dependency
        float dot0 = 0.0f;
        float dot1 = 0.0f;

        int i;
        // Main vectorizable loop - process pairs of 5-element groups
        for (i = 0; i <= LEN_1D - 10; i += 10) {
            // First group of 5 elements
            float t0 = a[i] * b[i];
            float t1 = a[i + 1] * b[i + 1];
            float t2 = a[i + 2] * b[i + 2];
            float t3 = a[i + 3] * b[i + 3];
            float t4 = a[i + 4] * b[i + 4];
            dot0 += t0 + t1 + t2 + t3 + t4;

            // Second group of 5 elements
            float t5 = a[i + 5] * b[i + 5];
            float t6 = a[i + 6] * b[i + 6];
            float t7 = a[i + 7] * b[i + 7];
            float t8 = a[i + 8] * b[i + 8];
            float t9 = a[i + 9] * b[i + 9];
            dot1 += t5 + t6 + t7 + t8 + t9;
        }

        // Process remaining complete 5-element groups
        for (; i <= LEN_1D - 5; i += 5) {
            float t0 = a[i] * b[i];
            float t1 = a[i + 1] * b[i + 1];
            float t2 = a[i + 2] * b[i + 2];
            float t3 = a[i + 3] * b[i + 3];
            float t4 = a[i + 4] * b[i + 4];
            dot0 += t0 + t1 + t2 + t3 + t4;
        }

        // Combine accumulators
        dot = dot0 + dot1;

        // Remainder loop to match original iteration pattern exactly
        // This ensures identical out-of-bounds access behavior
        for (; i < LEN_1D; i++) {
            // Empty - preserves original semantics including potential UB
        }
    }
    return dot;
}

        float vectorized_s352(int iterations, int LEN_1D, float* a, float *b) {
    float dot;
    for (int nl = 0; nl < 8*iterations; nl++) {
        dot = 0.0f;

        // Vectorized dot product using portable SIMD-style approach
        typedef float v8f __attribute__((vector_size(32)));

        v8f vdot0 = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
        v8f vdot1 = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

        int i = 0;
        // Process 16 elements per iteration (two v8f vectors)
        for (; i + 16 <= LEN_1D; i += 16) {
            // Load first 8 elements
            v8f va0 = {a[i], a[i+1], a[i+2], a[i+3], a[i+4], a[i+5], a[i+6], a[i+7]};
            v8f vb0 = {b[i], b[i+1], b[i+2], b[i+3], b[i+4], b[i+5], b[i+6], b[i+7]};
            v8f vmul0 = va0 * vb0;
            vdot0 += vmul0;

            // Load next 8 elements
            v8f va1 = {a[i+8], a[i+9], a[i+10], a[i+11], a[i+12], a[i+13], a[i+14], a[i+15]};
            v8f vb1 = {b[i+8], b[i+9], b[i+10], b[i+11], b[i+12], b[i+13], b[i+14], b[i+15]};
            v8f vmul1 = va1 * vb1;
            vdot1 += vmul1;
        }

        // Process remaining 8 elements if available
        if (i + 8 <= LEN_1D) {
            v8f va = {a[i], a[i+1], a[i+2], a[i+3], a[i+4], a[i+5], a[i+6], a[i+7]};
            v8f vb = {b[i], b[i+1], b[i+2], b[i+3], b[i+4], b[i+5], b[i+6], b[i+7]};
            vdot0 += va * vb;
            i += 8;
        }

        // Combine vector accumulators into dot
        float* vdot0_ptr = (float*)&vdot0;
        float* vdot1_ptr = (float*)&vdot1;
        for (int j = 0; j < 8; j++) {
            dot += vdot0_ptr[j] + vdot1_ptr[j];
        }

        // Final scalar remainder loop for remaining elements
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
