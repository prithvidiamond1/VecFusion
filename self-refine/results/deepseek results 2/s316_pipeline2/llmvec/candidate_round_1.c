#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s316(int iterations, int LEN_1D, float* a)
{
    float x;
    float min_val;

    for (int nl = 0; nl < iterations*5; nl++) {
        min_val = a[0];

        // Process first element separately
        x = min_val;

        // Main loop without branching - enables vectorization
        for (int i = 1; i < LEN_1D; ++i) {
            float current = a[i];
            // Use conditional move pattern (compiler can optimize to min instruction)
            if (current < min_val) {
                min_val = current;
            }
        }

        x = min_val;
    }
    return x;
}

        #include <float.h>

float vectorized_s316(int iterations, int LEN_1D, float* a)
{
    float x;
    float min_val;

    for (int nl = 0; nl < iterations * 5; nl++) {
        min_val = a[0];

        // Vector width: process 8 floats at a time
        typedef float float8 __attribute__((vector_size(32)));

        int i = 1;
        int vec_end = 1 + ((LEN_1D - 1) / 8) * 8;

        // Initialize 8-wide min vector
        float8 vmin = {min_val, min_val, min_val, min_val,
                       min_val, min_val, min_val, min_val};

        for (; i + 7 < LEN_1D; i += 8) {
            float8 va;
            __builtin_memcpy(&va, &a[i], sizeof(float8));
            // Element-wise min using ternary (compiler will use min instructions)
            vmin = va < vmin ? va : vmin;
        }

        // Reduce the vector to scalar
        float mins[8];
        __builtin_memcpy(mins, &vmin, sizeof(float8));
        for (int k = 0; k < 8; k++) {
            if (mins[k] < min_val) min_val = mins[k];
        }

        // Scalar tail
        for (; i < LEN_1D; i++) {
            float current = a[i];
            if (current < min_val) {
                min_val = current;
            }
        }

        x = min_val;
    }
    return x;
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
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float ret_scalar; float ret_vector;

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar));
                ret_scalar = s316(iterations, LEN_1D, a_scalar); ret_vector = vectorized_s316(iterations, LEN_1D, a_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
    fprintf(stderr, "Return mismatch on trial %d\n", trial);
    return 2;
}
                for (int i = 0; i < arr_len; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
