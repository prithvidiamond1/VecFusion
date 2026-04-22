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

        // Create multiple partial sums to break dependency chain
        float dot0 = 0.0f, dot1 = 0.0f, dot2 = 0.0f, dot3 = 0.0f, dot4 = 0.0f;

        // Process groups of 5 elements with 5 independent accumulators
        int i;
        for (i = 0; i + 4 < LEN_1D; i += 5) {
            // Each accumulator handles one element from the group
            dot0 += a[i] * b[i];
            dot1 += a[i + 1] * b[i + 1];
            dot2 += a[i + 2] * b[i + 2];
            dot3 += a[i + 3] * b[i + 3];
            dot4 += a[i + 4] * b[i + 4];
        }

        // Combine partial sums
        dot = dot0 + dot1 + dot2 + dot3 + dot4;

        // Handle remaining elements (if any)
        for (; i < LEN_1D; i++) {
            dot += a[i] * b[i];
        }
    }
    return dot;
}

        float vectorized_s352(int iterations, int LEN_1D, float* a, float *b) {
    float dot = 0.0f;

    for (int nl = 0; nl < 8 * iterations; nl++) {
        // Vectorized accumulation per outer iteration
        typedef float v8f __attribute__((vector_size(32), aligned(1)));
        v8f vdot = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

        // Process 8 elements at a time
        int i = 0;
        int main_len = LEN_1D - (LEN_1D % 8);

        for (; i < main_len; i += 8) {
            v8f va = *(v8f*)&a[i];
            v8f vb = *(v8f*)&b[i];
            vdot += va * vb;
        }

        // Horizontal sum of vector accumulator
        float* vdot_ptr = (float*)&vdot;
        float partial_dot = vdot_ptr[0] + vdot_ptr[1] + vdot_ptr[2] + vdot_ptr[3] +
                            vdot_ptr[4] + vdot_ptr[5] + vdot_ptr[6] + vdot_ptr[7];

        // Scalar tail for remaining elements
        for (; i < LEN_1D; i++) {
            partial_dot += a[i] * b[i];
        }

        // Accumulate across outer iterations
        dot += partial_dot;
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
